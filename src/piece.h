#pragma once

#include <vector>
#include <map>
#include <ostream>
#include <string>
#include <locale>

#include "board.h"
#include "locus.h"
#include "move.hpp"

class BoardSquare;

enum class Colour
{
    WHITE = 0,
    BLACK = 1
};

enum class PieceType
{
    PAWN,
    KING,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK
};

static const std::map<Colour, std::string> colourNames =
{
    {Colour::WHITE, "White"},
    {Colour::BLACK, "Black"}
};

static inline Colour getOppositeColour(Colour col)
{
    if (col == Colour::WHITE)
        return Colour::BLACK;
    else
        return Colour::WHITE;
}

typedef std::vector<std::vector<Direction>> PieceMovementSpec;

class Board;

class Piece
{
public:
    Piece(Colour colour);
    virtual moveList_t getCandidateMoves(const Board &b, Locus l) const = 0;
    virtual void printPiece(std::ostream &stream) const = 0;
    virtual PieceType getPieceType(void) const  = 0;
    int getValue(void) const;
    Colour getColour(void) const;
    bool operator==(const Piece &other) const;
protected:
    moveList_t applyTranslationSpec(const Board &b, Locus &from,
                                    const PieceMovementSpec &ms,
                                    bool singularTransform) const;
    char formatPieceChar(char pieceName) const;
private:
    virtual unsigned int getValueMagnitude(void) const = 0;
    Colour colour_;
};
