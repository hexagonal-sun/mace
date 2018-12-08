#pragma once
#include <array>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <vector>

#include "boardSquare.h"

enum class Colour;

typedef std::unordered_map<Locus, BoardSquare> board_t;

class Board
{
public:
    Board(board_t b, Colour nextMoveColour);
    static Board getStartingBoard(void);
    Board move(std::string from, std::string to) const;
    Board move(const BoardSquare &from, const BoardSquare &to) const;
    Board move(Locus from, Locus to) const;
    bool canMoveToSquare(Locus l, Colour c) const;
    std::vector<Board> getAllCandidateMoves(void) const;
    const BoardSquare & getSquare(std::string name) const;
    void printBoard(std::ostream &stream) const;
    const BoardSquare & operator[](const Locus &l) const;
private:
    static board_t getEmptyBoard(void);
    static BoardSquare & getSquareFromLocus(board_t &b, Locus loc);
    void evaluatePosition(void);
    int evaluation_;
    Colour nextMoveColour_;
    board_t board_;
};
