#pragma once
#include <array>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <vector>

#include "boardSquare.h"
#include "piece.h"

enum class Colour;
enum class PieceType;

typedef std::unordered_map<Locus, BoardSquare> board_t;
typedef std::tuple<Locus, Locus> Move;

class Board
{
public:
    static Board getStartingBoard(void);
    Board move(std::string from, std::string to) const;
    Board move(const BoardSquare &from, const BoardSquare &to) const;
    Board move(Locus from, Locus to) const;
    Board move(Move m) const;
    bool validateMove(std::string from, std::string to) const;
    bool validateMove(const Locus &from, const Locus &to) const;
    Colour getNextMoveColour(void) const;
    const std::vector<Move> &getMoveList(void) const;
    const int getEvaluation(void) const;
    std::vector<Move> getAllCandidateMoves(void) const;
    const BoardSquare & getSquare(std::string name) const;
    void printBoard(std::ostream &stream) const;
    const BoardSquare & operator[](const Locus &l) const;
    bool operator==(const Board& other) const;
private:
    Board(board_t b, Colour nextMoveColour, std::vector<Move> move);
    Board(board_t b, Colour nextMoveColour);
    static board_t getEmptyBoard(void);
    static BoardSquare & getSquareFromLocus(board_t &b, Locus loc);
    std::vector<Locus> locatePiece(Colour c, PieceType t) const;
    void evaluatePosition(void);
    std::vector<Move> moves_;
    int evaluation_;
    Colour nextMoveColour_;
    board_t board_;
};
