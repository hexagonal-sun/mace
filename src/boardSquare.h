#pragma once
#include <ostream>
#include <memory>

#include "locus.h"

class Piece;

class BoardSquare
{
public:
    BoardSquare(Rank r, File f);
    void setPiece(std::shared_ptr<Piece> piece);
    void setEmpty(void);
    bool isOccupied() const;
    const Locus getLocus(void) const;
    void printSquare(std::ostream &stream) const;
    std::shared_ptr<Piece> getPiece(void) const;
    bool operator==(const BoardSquare &other) const;
private:
    bool isOccupied_;
    Locus loc_;
    std::shared_ptr<Piece> piece_;
};
