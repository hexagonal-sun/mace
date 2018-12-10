#pragma once

#include <vector>
#include <map>
#include <ostream>
#include <memory>
#include <string>
#include <locale>

#include "board.h"
#include "locus.h"

class BoardSquare;

enum class Colour
{
    WHITE,
    BLACK
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
typedef std::tuple<Locus, Locus> Move;

class Piece
{
public:
    Piece(Colour colour);
    virtual std::vector<Move> getCandidateMoves(const Board &b, Locus l) const = 0;
    virtual void printPiece(std::ostream &stream) const = 0;
    virtual PieceType getPieceType(void) const  = 0;
    int getValue(void) const;
    bool canMoveToSquare(const Board &b, Locus l, Colour c) const;
    void setSquare(std::shared_ptr<BoardSquare>);
    Colour getColour(void) const;
    bool operator==(const Piece &other) const;
protected:
    std::vector<Move> applyTranslationSpec(const Board &b, Locus &from,
                                           const PieceMovementSpec &ms,
                                           bool singularTransform) const;
    char formatPieceChar(char pieceName) const;
    std::shared_ptr<BoardSquare> square_;
private:
    virtual unsigned int getValueMagnitude(void) const = 0;
    Colour colour_;
};
