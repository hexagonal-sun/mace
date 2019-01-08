#include <algorithm>

#include "moveTypes/orthodiagonal.hpp"
#include "moveTypes/orthogonal.hpp"
#include "moveTypes/knight.hpp"
#include "moveTypes/diagonal.hpp"
#include "moveGen.h"
#include "board.h"
#include "board.h"
#include "mover.hpp"

template <typename T, size_t N>
static void
applyTranslationSpec(const Board &b, Locus &from,
                     SquareState fromSquare,
                     const std::array<T, N> &ms,
                     bool singularTransform,
                     moveList_t &takeMoves,
                     moveList_t &quietMoves)
{
    auto movingPieceColour = fromSquare.getColour();

    for (const auto &dir : ms)
    {
        Locus l = from;

        while (1)
        {
            l += dir;

            if (!l.isValid())
                goto nextDir;

            const auto &square = b[l];

            if (square.isOccupied() &&
                square.getColour() == movingPieceColour)
                goto nextDir;

            auto moveType = square.isOccupied() ?
                MoveType::TAKE : MoveType::UNOCCUPIED;

            moveList_t &moveList = moveType == MoveType::TAKE ?
                takeMoves : quietMoves;

            moveList.push_back(Move(from, l, moveType));

            if (singularTransform || moveType == MoveType::TAKE)
                goto nextDir;
        }

    nextDir:;
    }
}

static void
genMovesBishop(const Board &b, Locus from, SquareState sq, moveList_t &takeMoves,
               moveList_t &quietMoves)
{
    applyTranslationSpec(b, from, sq, diagonalMoves, false, takeMoves,
                         quietMoves);
}

static void genCastlingMove(moveList_t &moves, Colour ourColour, const Board &b,
                            Locus from, Direction d, MoveType type)
{
    Locus l = from;
    auto opponentsColour = ~ourColour;

    while (1) {
        l += d;

        if (!l.isValid())
            throw std::logic_error("Attempted to castle when rook isn't present");

        // If this square is empty, carry on checking for other
        // pieces.  Eventually we will hit the rook.
        if (!b[l].isOccupied())
            continue;

        const auto pieceType = b[l].getPieceType();

        // If any other piece is in the way, we can't castle.
        if (pieceType != PieceType::ROOK)
            return;

        // Ensure that we don't permit castling when an opponent's
        // rook is on the same rank, or our queen side rank has moved
        // in between the king-side rook and the King.
        if (!(l.getFile() == File::A || l.getFile() == File::H))
            return;

        auto castlingKingsLocus = from + d + d;

        // Ensure that the squares the king will travel
        // through aren't under attack.
        //
        // NOTE: we don't need to check whether `castlingKingsLocus'
        // is being attacked as that move will be discarded by
        // getAllCandidateMoves ensuring that we can't move the king
        // into check.
        if (b.isSquareUnderAttack(from,      opponentsColour) ||
            b.isSquareUnderAttack(from + d,  opponentsColour))
            return;

        // We can legally now castle.
        moves.push_back(Move(from, castlingKingsLocus, type));

        return;
    }
}

static void
addCastlingMoves(moveList_t &moves, Colour ourColour, const Board &b,
                 Locus from, bool canCastleKingSide, bool canCastleQueenSide)
{
    if (canCastleKingSide)
        genCastlingMove(moves, ourColour, b, from, Direction::EAST(),
                        MoveType::CASTLE_KINGSIDE);

    if (canCastleQueenSide)
        genCastlingMove(moves, ourColour, b, from, Direction::WEST(),
                        MoveType::CASTLE_QUEENSIDE);
}

static void
genMovesKing(const Board &b, Locus from, SquareState sq, moveList_t &takeMoves,
             moveList_t &quietMoves)
{
    auto cr = b.getCastlingRights();
    const auto &colourMask = getCastlingMask(sq.getColour());

    applyTranslationSpec(b, from, sq, orthoDiagonalMoves, true, takeMoves,
                         quietMoves);

    if (!(cr & colourMask).any())
        // No castling rights are open to us, just return king's
        // standard moves.
        return;

    addCastlingMoves(quietMoves, sq.getColour(), b, from,
                     (cr & colourMask & getKingSideMask()).any(),
                     (cr & colourMask & getQueenSideMask()).any());
}

static void
genMovesKnight(const Board &b, Locus from, SquareState sq, moveList_t &takeMoves,
               moveList_t &quietMoves)
{
    applyTranslationSpec(b, from, sq, knightMoves, true, takeMoves, quietMoves);
}

static void addPawnPromotions(moveList_t &moves, Locus from, Locus to)
{
    for (const auto promoType : {MoveType::PROMOTE_BISHOP,
                                 MoveType::PROMOTE_KNIGHT,
                                 MoveType::PROMOTE_QUEEN,
                                 MoveType::PROMOTE_ROOK})
        moves.push_back(Move(from, to, promoType));
}

static void
genMovesPawn(const Board &b, Locus from, SquareState sq, moveList_t &takeMoves,
             moveList_t &quietMoves)
{
    const auto pawnColour = sq.getColour();
    const auto dir = pawnColour == Colour::WHITE ? Direction::NORTH() : Direction::SOUTH();
    const auto promotionRank = pawnColour == Colour::WHITE ? Rank::EIGHT : Rank::ONE;
    const auto startingRank = pawnColour == Colour::WHITE ? Rank::TWO : Rank::SEVEN;

    auto newLoc = from + dir;

    if (!b[newLoc].isOccupied()) {
        if (newLoc.getRank() == promotionRank)
            addPawnPromotions(takeMoves, from, newLoc);
        else
            quietMoves.push_back(Move(from, newLoc, MoveType::UNOCCUPIED));

        // We can only move a pawn forward two squares if it isn't
        // obstructed at `newLoc' and it's on it's starting rank.
        if (from.getRank() == startingRank) {
            auto doubleMove = newLoc + dir;
            if (!b[doubleMove].isOccupied())
                quietMoves.push_back(Move(from, doubleMove,
                                          MoveType::ENPASSANT_ADVANCE));
        }
    }

    // See if we can take any pieces.
    for (const auto takeDir : {Direction::EAST(), Direction::WEST()}) {
        const auto takeLoc = from + dir + takeDir;
        auto enPassantRank = pawnColour == Colour::WHITE ?
            Rank::FIVE : Rank::FOUR;

        if (!takeLoc.isValid())
            continue;

        if (from.getRank() == enPassantRank &&
            takeLoc == b.getEnPassantLocus())
            takeMoves.push_back(Move(from, takeLoc,
                                     MoveType::ENPASSANT_TAKE));

        if (b[takeLoc].isOccupied() &&
            b[takeLoc].getColour() == ~pawnColour) {
            if (takeLoc.getRank() == promotionRank)
                addPawnPromotions(takeMoves, from, takeLoc);
            else
                takeMoves.push_back(Move(from, takeLoc, MoveType::TAKE));
        }
    }
}

static void
genMovesQueen(const Board &b, Locus from, SquareState sq, moveList_t &takeMoves,
              moveList_t &quietMoves)
{
    applyTranslationSpec(b, from, sq, orthoDiagonalMoves, false, takeMoves,
                         quietMoves);
}

static void
genMovesRook(const Board &b, Locus from, SquareState sq, moveList_t &takeMoves,
             moveList_t &quietMoves)
{
    applyTranslationSpec(b, from, sq, orthogonalMoves, false, takeMoves,
                         quietMoves);
}

static void
genMoves(const Board &b, Locus from, SquareState sq, moveList_t &takeMoves,
         moveList_t &quietMoves)
{
    auto pieceType = sq.getPieceType();

    switch (pieceType)
    {
    case PieceType::BISHOP:
        genMovesBishop(b, from, sq, takeMoves, quietMoves);
        break;
    case PieceType::KING:
        genMovesKing(b, from, sq, takeMoves, quietMoves);
        break;
    case PieceType::KNIGHT:
        genMovesKnight(b, from, sq, takeMoves, quietMoves);
        break;
    case PieceType::PAWN:
        genMovesPawn(b, from, sq, takeMoves, quietMoves);
        break;
    case PieceType::QUEEN:
        genMovesQueen(b, from, sq, takeMoves, quietMoves);
        break;
    case PieceType::ROOK:
        genMovesRook(b, from, sq, takeMoves, quietMoves);
        break;
    case PieceType::UNOCCUPIED:
        return;
    }
}

static const std::array<std::array<int, 6>, 6> MvvLvaScores =
{{
    //      P, K  Q, B, N  R,
    /*P*/ { 0, 0,-9,-3,-2,-5},
    /*K*/ { 0, 0, 0, 0, 0, 0},
    /*Q*/ { 8, 0, 4, 6, 7, 5},
    /*B*/ { 3, 0,-6, 1, 1,-2},
    /*N*/ { 2, 0,-6, 0, 1,-2},
    /*R*/ { 5, 0,-5, 3, 4, 2},
}};

static int calculateMVVLVA(const Move &m, const Board &b)
{
    auto takingPiece = b[m.getFrom()].getPieceType();
    auto captiredPiece = b[m.getTo()].getPieceType();

    if (isPromotion(m.getType()))
        return 9;

    if (m.getType() == MoveType::ENPASSANT_TAKE)
        return 0;

    return MvvLvaScores[getPieceTypeIdx(captiredPiece)]
                       [getPieceTypeIdx(takingPiece)];
}

moveList_t
MoveGen::getLegalMoves(Board &b, Move hashMove)
{
    moveList_t quietMoves;
    moveList_t takeMoves;
    const auto colourToMove = b.getNextMoveColour();

    for (const auto [square, loc] : b.getChessBoard())
        if (square.isOccupied() &&
            square.getColour() == colourToMove)
            genMoves(b, loc, square , takeMoves, quietMoves);

    // MVV-LVA sorting for taking moves.
    std::sort(takeMoves.begin(), takeMoves.end(),
              [&](const Move &ma, const Move &mb)
    {
        return calculateMVVLVA(ma, b) > calculateMVVLVA(mb, b);
    });

    // Append the quiet moves onto the end of take moves to produce
    // the full move list.
    takeMoves.insert(takeMoves.end(), quietMoves.begin(), quietMoves.end());

    // Prune non-legal pseudo moves.
    takeMoves.erase(std::remove_if(takeMoves.begin(),
                                   takeMoves.end(),
                                   [&](const Move &move)
    {
        Mover<MoverType::REVERT> m(move, b);
        return b.isInCheck(colourToMove);
    }), takeMoves.end());

    if (hashMove.isValid())
    {
        // Attempt to find the hashmove in the move list.
        auto hm = std::find(takeMoves.begin(), takeMoves.end(),
                            hashMove);

        // If we find it, make it the first element to be searched.
        if (hm != takeMoves.end())
        {
            takeMoves.erase(hm);

            takeMoves.insert(takeMoves.begin(), hashMove);
        }
    }

    return takeMoves;
}
