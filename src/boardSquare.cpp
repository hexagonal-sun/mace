#include "boardSquare.h"
#include "piece.h"

BoardSquare::BoardSquare(Locus l)
    : loc_(l),
      isOccupied_(false)
{
}

void BoardSquare::setPiece(std::shared_ptr<Piece> piece)
{
    piece_ = piece;
    isOccupied_ = true;
}

void BoardSquare::setEmpty(void)
{
    piece_ = nullptr;
    isOccupied_ = false;
}

const Locus BoardSquare::getLocus(void) const
{
    return loc_;
}

bool BoardSquare::isOccupied() const
{
    return isOccupied_;
}

void BoardSquare::printSquare(std::ostream &stream) const
{
    if (!isOccupied_)
        stream << ".";
    else
        piece_->printPiece(stream);
}

std::shared_ptr<Piece> BoardSquare::getPiece(void) const
{
    if (!isOccupied_)
        throw std::domain_error("Attempted to get piece on an empty square");

    return piece_;
}

bool BoardSquare::operator==(const BoardSquare &other) const
{
    if (isOccupied_ != other.isOccupied())
        return false;

    if (!isOccupied_)
        return true;

    return *getPiece() == *other.getPiece();
}

SquareType BoardSquare::getSquareType(Colour c) const
{
    if (!isOccupied())
        return SquareType::EMPTY;

    // We can take opposite coloured pieces
    if (getPiece()->getColour() != c)
        return SquareType::TAKE;

    return SquareType::OCCUPIED;
}
