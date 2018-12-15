#pragma once
#include <array>
#include <ostream>
#include <array>
#include <functional>
#include <vector>

#include "boardSquare.h"
#include "chessBoard.h"
#include "piece.h"
#include "move.hpp"

enum class Colour;
enum class PieceType;

typedef std::function<bool(Piece *piece,
                           const moveList_t &)> moveCallback_t;

typedef boost::container::small_vector<Locus, 8> locusList_t;

class Board
{
public:
    static Board getStartingBoard(void);
    Move validateMove(std::string from, std::string to);
    Move validateMove(const Locus &from, const Locus &to);
    const Colour getNextMoveColour(void) const;
    Colour &getNextMoveColour(void);
    const int getEvaluation(void) const;
    const bool isInCheck(Colour kingsColour) const;
    const Locus &getKingLocus(Colour c) const;
    Locus &getKingLocus(Colour c);
    const Locus &getEnPassantLocus(void) const;
    Locus &getEnPassantLocus(void);
    moveList_t getAllCandidateMoves(void);
    const BoardSquare & getSquare(std::string name) const;
    void printBoard(std::ostream &stream) const;
    const BoardSquare & operator[](const Locus &l) const;
    BoardSquare & operator[](const Locus &l);
    int perft(int depth);
private:
    Board(Colour nextMoveColour);
    Locus enPassantCapture_;
    std::array<Locus, 2> kingLocus_;
    void forEachPieceMoves(Colour c, moveCallback_t callback) const;
    bool isPieceUnderAttack(Locus l) const;
    locusList_t locatePiece(Colour c, PieceType t) const;
    Colour nextMoveColour_;
    ChessBoard board_;
};
