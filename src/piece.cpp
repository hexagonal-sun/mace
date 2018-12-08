#include "piece.h"

Piece::Piece(Colour colour)
        : colour_(colour)
{
}

Colour Piece::getColour(void) const
{
    return colour_;
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
