#include "piece.h"

Piece::Piece(Colour colour)
        : colour_(colour)
{
}

Colour Piece::getColour(void) const
{
    return colour_;
}

moveList_t Piece::applyTranslationSpec(const Board &b, Locus &from,
                                              const PieceMovementSpec &ms,
                                              bool singularTransform) const
{
    moveList_t ret;

    for (const auto &dirs : ms)
    {
        auto l = from;
        while (true) {
            for (const auto &dir : dirs) {
                l = l.translate(dir);

                if (!l.isValid())
                    goto nextDir;
            }

            auto squareType = b[l].getSquareType(getColour());

            if (squareType == SquareType::OCCUPIED)
                goto nextDir;

            auto moveType = squareType == SquareType::EMPTY ?
                MoveType::UNOCCUPIED : MoveType::TAKE;

            ret.push_back(Move(from, l, moveType));

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
