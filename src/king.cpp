#include "king.h"
#include "move.hpp"
#include "castlingRights.hpp"

King::King(Colour col)
    : Piece(col)
{
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
        l = l.translate(d);

        if (!l.isValid())
            throw std::logic_error("Attempted to castle when rook isn't present");

        // If this square is empty, carry on checking for other
        // pieces.  Eventually we will hit the rook.
        if (!b[l].isOccupied())
            continue;

        const auto piece = b[l].getPiece();

        // If any other piece is in the way, we can't castle.
        if (piece->getPieceType() != PieceType::ROOK)
            return;

        // Ensure that we don't permit castling when an opponent's
        // rook is on the same rank, or our queen side rank has moved
        // in between the king-side rook and the King.
        if (!(l.getFile() == File::A || l.getFile() == File::H))
            return;

        auto castlingKingsLocus = from.translate(d).translate(d);

        // Ensure that the squares the king will travel
        // through aren't under attack.
        //
        // NOTE: we don't need to check whether `castlingKingsLocus'
        // is being attacked as that move will be discarded by
        // getAllCandidateMoves ensuring that we can't move the king
        // into check.
        if (b.isSquareUnderAttack(from,               opponentsColour) ||
            b.isSquareUnderAttack(from.translate(d),  opponentsColour))
            return;

        // We can legally now castle.
        moves.push_back(Move(from, castlingKingsLocus, type));

        return;
    }
}

static void addCastlingMoves(moveList_t &moves,
                             Colour ourColour,
                             const Board &b,
                             Locus from,
                             bool canCastleKingSide,
                             bool canCastleQueenSide)
{
    if (canCastleKingSide)
        genCastlingMove(moves, ourColour, b, from, Direction::EAST,
                        MoveType::CASTLE_KINGSIDE);

    if (canCastleQueenSide)
        genCastlingMove(moves, ourColour, b, from, Direction::WEST,
                        MoveType::CASTLE_QUEENSIDE);
}

moveList_t King::getCandidateMoves(const Board &b, Locus from) const
{
    moveList_t ret;

    auto cr = b.getCastlingRights();
    const auto &mask = getCastlingMask(getColour());

    static const PieceMovementSpec kingMovementSpec = {
        {Direction::NORTH},
        {Direction::EAST},
        {Direction::SOUTH},
        {Direction::WEST},
        {Direction::NORTH, Direction::EAST},
        {Direction::NORTH, Direction::WEST},
        {Direction::SOUTH, Direction::EAST},
        {Direction::SOUTH, Direction::WEST}
    };

    ret = applyTranslationSpec(b, from, kingMovementSpec, true);

    if (!(cr & mask).any())
        // No castling rights are open to us, just return king's
        // standard moves.
        return ret;

    addCastlingMoves(ret, getColour(), b, from,
                     (mask & getKingSideMask()).any(),
                     (mask & getQueenSideMask()).any());

    return ret;
}

void King::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('k');
}

unsigned int King::getValueMagnitude(void) const
{
    return 1024;
}

PieceType King::getPieceType(void) const
{
    return PieceType::KING;
}
