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
    bool validateMove(std::string from, std::string to) const;
    bool validateMove(const Locus &from, const Locus &to) const;
    const std::vector<Move> &getMoveList(void) const;
    const Colour getNextMoveColour(void) const;
    Colour &getNextMoveColour(void);
    const int getEvaluation(void) const;
    std::vector<Move> getAllCandidateMoves(void) const;
    const BoardSquare & getSquare(std::string name) const;
    void printBoard(std::ostream &stream) const;
    const BoardSquare & operator[](const Locus &l) const;
    BoardSquare & operator[](const Locus &l);
    bool operator==(const Board& other) const;
private:
    Board(board_t b, Colour nextMoveColour);
    bool isPieceUnderAttack(Locus l) const;
    static board_t getEmptyBoard(void);
    static BoardSquare & getSquareFromLocus(board_t &b, Locus loc);
    std::vector<Locus> locatePiece(Colour c, PieceType t) const;
    std::vector<Move> moves_;
    Colour nextMoveColour_;
    board_t board_;
};
