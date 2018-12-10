#include "queen.h"

Queen::Queen(Colour col)
    : Piece(col)
{
}

std::vector<Board> Queen::getCandidateMoves(const Board &b, Locus from) const
{
    static const PieceMovementSpec queenMovementSpec = {
        {Direction::NORTH},
        {Direction::EAST},
        {Direction::SOUTH},
        {Direction::WEST},
        {Direction::NORTH, Direction::EAST},
        {Direction::NORTH, Direction::WEST},
        {Direction::SOUTH, Direction::EAST},
        {Direction::SOUTH, Direction::WEST}
    };

    return applyTranslationSpec(b, from, queenMovementSpec, false);
}

void Queen::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('q');
}

unsigned int Queen::getValueMagnitude(void) const
{
    return 9;
}

PieceType Queen::getPieceType(void) const
{
    return PieceType::QUEEN;
}
