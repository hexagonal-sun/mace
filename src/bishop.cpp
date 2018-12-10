#include "bishop.h"

Bishop::Bishop(Colour col)
    : Piece(col)
{
}

std::vector<Move> Bishop::getCandidateMoves(const Board &b, Locus from) const
{
    static const PieceMovementSpec bishopSpec = {
        {Direction::NORTH, Direction::EAST},
        {Direction::NORTH, Direction::WEST},
        {Direction::SOUTH, Direction::EAST},
        {Direction::SOUTH, Direction::WEST}
    };

    return applyTranslationSpec(b, from, bishopSpec, false);
}

void Bishop::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('b');
}

unsigned int Bishop::getValueMagnitude(void) const
{
    return 3;
}

PieceType Bishop::getPieceType(void) const
{
    return PieceType::BISHOP;
}
