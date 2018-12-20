#pragma once

#include <vector>
#include <map>
#include <ostream>
#include <string>
#include <locale>

#include "colour.hpp"
#include "locus.h"
#include "move.hpp"

class BoardSquare;

enum class PieceType
{
    PAWN,
    KING,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK
};

class Board;

class Piece
{
public:
    Piece(Colour colour);
    virtual ~Piece() = default;
    virtual moveList_t getCandidateMoves(const Board &b, Locus l) const = 0;
    virtual void printPiece(std::ostream &stream) const = 0;
    virtual PieceType getPieceType(void) const  = 0;
    int getValue(void) const;
    Colour getColour(void) const;
    bool operator==(const Piece &other) const;
protected:
    template <typename T, size_t N>
    moveList_t applyTranslationSpec(const Board &b, Locus &from,
                                    const std::array<T, N> &ms,
                                    bool singularTransform) const
        {
            moveList_t ret;

            for (const auto &dir : ms)
            {
                Locus l = from;

                while (1)
                {
                    l += dir;

                    if (!l.isValid())
                        goto nextDir;

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

    char formatPieceChar(char pieceName) const;
private:
    virtual unsigned int getValueMagnitude(void) const = 0;
    Colour colour_;
};
