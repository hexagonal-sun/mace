#include "rook.h"

Rook::Rook(Colour col)
    : Piece(col)
{
}

moveList_t Rook::getCandidateMoves(const Board &b, Locus from) const
{
    static const PieceMovementSpec rookSpec = {
        {Direction::NORTH},
        {Direction::EAST},
        {Direction::SOUTH},
        {Direction::WEST}
    };

    return applyTranslationSpec(b, from, rookSpec, false);
}

void Rook::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('r');
}

unsigned int Rook::getValueMagnitude(void) const
{
    return 5;
}

PieceType Rook::getPieceType(void) const
{
    return PieceType::ROOK;
}
