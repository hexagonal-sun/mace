#include "knight.h"

Knight::Knight(Colour col)
    : Piece(col)
{
}

std::vector<Board> Knight::getCandidateMoves(const Board &b, Locus from) const
{
    std::vector<Board> ret;

    static const PieceMovementSpec knightSpec = {
        {Direction::NORTH, Direction::NORTH, Direction::EAST},
        {Direction::NORTH, Direction::NORTH, Direction::WEST},
        {Direction::EAST, Direction::EAST, Direction::NORTH},
        {Direction::EAST, Direction::EAST, Direction::SOUTH},
        {Direction::SOUTH, Direction::SOUTH, Direction::EAST},
        {Direction::SOUTH, Direction::SOUTH, Direction::WEST},
        {Direction::WEST, Direction::WEST, Direction::NORTH},
        {Direction::WEST, Direction::WEST, Direction::SOUTH}
    };

    return applyTranslationSpec(b, from, knightSpec, true);
}

void Knight::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('n');
}

unsigned int Knight::getValueMagnitude(void) const
{
    return 3;
}

PieceType Knight::getPieceType(void) const
{
    return PieceType::KNIGHT;
}
