#pragma once
#include <ostream>

#include "locus.h"

class Piece;
enum class Colour;

enum class SquareType
{
    EMPTY,
    TAKE,
    OCCUPIED
};

class BoardSquare
{
public:
    BoardSquare() {};
    BoardSquare(Locus l);
    void setPiece(Piece *piece);
    void setEmpty(void);
    bool isOccupied() const;
    const Locus getLocus(void) const;
    SquareType getSquareType(Colour c) const;
    void printSquare(std::ostream &stream) const;
    Piece *getPiece(void) const;
    bool operator==(const BoardSquare &other) const;
private:
    bool isOccupied_;
    Locus loc_;
    Piece *piece_;
};
