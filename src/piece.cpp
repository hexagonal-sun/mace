#include "piece.h"

Piece::Piece(Colour colour)
        : colour_(colour)
{
}

Colour Piece::getColour(void) const
{
    return colour_;
}

std::vector<Move> Piece::applyTranslationSpec(const Board &b, Locus &from,
                                              const PieceMovementSpec &ms,
                                              bool singularTransform) const
{
    std::vector<Move> ret;

    for (const auto &dirs : ms)
    {
        auto l = from;
        while (true) {
            try {
                for (const auto &dir : dirs)
                    l = l.translate(dir);
            } catch (std::domain_error &e) {
                goto nextDir;
            }

            auto squareType = b[l].getSquareType(getColour());

            if (squareType == SquareType::OCCUPIED)
                goto nextDir;

            ret.push_back(std::make_tuple(from, l));

            if (singularTransform ||
                squareType == SquareType::TAKE)
                goto nextDir;
        }
    nextDir:;
    }

    return ret;
}

char Piece::formatPieceChar(char pieceName) const
{
    if (getColour() == Colour::BLACK)
        return std::tolower(pieceName);
    else
        return std::toupper(pieceName);
}

void Piece::setSquare(std::shared_ptr<BoardSquare> sq)
{
    square_ = sq;
}

int Piece::getValue(void) const
{
    unsigned int mag = getValueMagnitude();

    if (colour_ == Colour::WHITE)
        return mag;
    else
        return -mag;
}

bool Piece::operator==(const Piece &other) const
{
    if (typeid(*this) != typeid(other))
        return false;

    return getColour() == other.getColour();
}
