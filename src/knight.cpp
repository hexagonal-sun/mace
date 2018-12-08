#include "knight.h"

Knight::Knight(Colour col)
    : Piece(col)
{
}

std::vector<Board> Knight::getCandidateMoves(Board &b, Locus from) const
{
    std::vector<Board> ret;

    static const std::vector<std::vector<Direction>> translations = {
        {Direction::NORTH, Direction::NORTH, Direction::EAST},
        {Direction::NORTH, Direction::NORTH, Direction::WEST},
        {Direction::EAST, Direction::EAST, Direction::NORTH},
        {Direction::EAST, Direction::EAST, Direction::SOUTH},
        {Direction::SOUTH, Direction::SOUTH, Direction::EAST},
        {Direction::SOUTH, Direction::SOUTH, Direction::WEST},
        {Direction::WEST, Direction::WEST, Direction::NORTH},
        {Direction::WEST, Direction::WEST, Direction::SOUTH}
    };

    for (const auto &translation : translations) {
        auto d = from;

        for (const auto &dir : translation) {
            try {
                d = d.translate(dir);
            } catch (std::domain_error &e) {
                goto nextTranslation;
            }
        }

        if (b.canMoveToSquare(d, getColour()))
            ret.push_back(b.move(from, d));

    nextTranslation:;
    }

    return ret;
}

void Knight::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('n');
}

unsigned int Knight::getValueMagnitude(void) const
{
    return 3;
}
