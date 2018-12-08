#include "king.h"

King::King(Colour col)
    : Piece(col)
{
}

std::vector<Board> King::getCandidateMoves(const Board &b, Locus from) const
{
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

    return applyTranslationSpec(b, from, kingMovementSpec, true);
}

void King::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('k');
}

unsigned int King::getValueMagnitude(void) const
{
    return 1024;
}
