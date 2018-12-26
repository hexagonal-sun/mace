#include "moveGen.h"
#include "board.h"
#include "moveTypes/orthodiagonal.hpp"
#include "moveTypes/orthogonal.hpp"
#include "moveTypes/knight.hpp"
#include "moveTypes/diagonal.hpp"
#include "board.h"

template <typename T, size_t N>
static moveList_t
applyTranslationSpec(const Board &b, Locus &from,
                     SquareState fromSquare,
                     const std::array<T, N> &ms,
                     bool singularTransform)
{
    moveList_t ret;
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

            ret.push_back(Move(from, l, moveType));

            if (singularTransform || moveType == MoveType::TAKE)
                goto nextDir;
        }

    nextDir:;
    }

    return ret;
}

static moveList_t
genMovesBishop(const Board &b, Locus from, SquareState sq)
{
    return applyTranslationSpec(b, from, sq, diagonalMoves, false);
}

static void genCastlingMove(moveList_t &moves,
                            Colour ourColour,
                            const Board &b,
                            Locus from,
                            Direction d,
                            MoveType type)
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

static moveList_t
genMovesKing(const Board &b, Locus from, SquareState sq)
{
    moveList_t ret;

    auto cr = b.getCastlingRights();
    const auto &colourMask = getCastlingMask(sq.getColour());

    ret = applyTranslationSpec(b, from, sq, orthoDiagonalMoves, true);

    if (!(cr & colourMask).any())
        // No castling rights are open to us, just return king's
        // standard moves.
        return ret;

    addCastlingMoves(ret, sq.getColour(), b, from,
                     (cr & colourMask & getKingSideMask()).any(),
                     (cr & colourMask & getQueenSideMask()).any());

    return ret;
}

static moveList_t
genMovesKnight(const Board &b, Locus from, SquareState sq)
{
    return applyTranslationSpec(b, from, sq, knightMoves, true);
}

static void addPawnPromotions(moveList_t &ret, Locus from, Locus to)
{
    for (const auto promoType : {MoveType::PROMOTE_BISHOP,
                                 MoveType::PROMOTE_KNIGHT,
                                 MoveType::PROMOTE_QUEEN,
                                 MoveType::PROMOTE_ROOK})
        ret.push_back(Move(from, to, promoType));
}

static moveList_t
genMovesPawn(const Board &b, Locus from, SquareState sq)
{
    const auto pawnColour = sq.getColour();
    const auto dir = pawnColour == Colour::WHITE ? Direction::NORTH() : Direction::SOUTH();
    const auto promotionRank = pawnColour == Colour::WHITE ? Rank::EIGHT : Rank::ONE;
    const auto startingRank = pawnColour == Colour::WHITE ? Rank::TWO : Rank::SEVEN;
    moveList_t ret;

    auto newLoc = from + dir;

    if (!b[newLoc].isOccupied()) {
        if (newLoc.getRank() == promotionRank)
            addPawnPromotions(ret, from, newLoc);
        else
            ret.push_back(Move(from, newLoc, MoveType::UNOCCUPIED));

        // We can only move a pawn forward two squares if it isn't
        // obstructed at `newLoc' and it's on it's starting rank.
        if (from.getRank() == startingRank) {
            auto doubleMove = newLoc + dir;
            if (!b[doubleMove].isOccupied())
                ret.push_back(Move(from, doubleMove,
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
            ret.push_back(Move(from, takeLoc,
                               MoveType::ENPASSANT_TAKE));

        if (b[takeLoc].isOccupied() &&
            b[takeLoc].getColour() == ~pawnColour) {
            if (takeLoc.getRank() == promotionRank)
                addPawnPromotions(ret, from, takeLoc);
            else
                ret.push_back(Move(from, takeLoc, MoveType::TAKE));
        }
    }

    return ret;
}

static moveList_t
genMovesQueen(const Board &b, Locus from, SquareState sq)
{
    return applyTranslationSpec(b, from, sq, orthoDiagonalMoves, false);
}

static moveList_t
genMovesRook(const Board &b, Locus from, SquareState sq)
{
    return applyTranslationSpec(b, from, sq, orthogonalMoves, false);
}

moveList_t
MoveGen::genMoves(const Board &b, Locus from, SquareState sq)
{
    auto pieceType = sq.getPieceType();

    switch (pieceType)
    {
    case PieceType::BISHOP:
        return genMovesBishop(b, from, sq);
    case PieceType::KING:
        return genMovesKing(b, from, sq);
    case PieceType::KNIGHT:
        return genMovesKnight(b, from, sq);
    case PieceType::PAWN:
        return genMovesPawn(b, from, sq);
    case PieceType::QUEEN:
        return genMovesQueen(b, from, sq);
    case PieceType::ROOK:
        return genMovesRook(b, from, sq);
    case PieceType::UNOCCUPIED:
        return moveList_t();
    }
}
