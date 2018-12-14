#pragma once
#include <array>
#include <memory>
#include <ostream>
#include <array>
#include <functional>
#include <vector>

#include "boardSquare.h"
#include "chessBoard.h"
#include "piece.h"

enum class Colour;
enum class PieceType;

typedef std::tuple<Locus, Locus> Move;
typedef boost::container::small_vector<Move, 64> moveList_t;
typedef std::function<bool(std::shared_ptr<Piece> piece,
                           const moveList_t &)> moveCallback_t;

class Board
{
public:
    static Board getStartingBoard(void);
    bool validateMove(std::string from, std::string to);
    bool validateMove(const Locus &from, const Locus &to);
    const std::vector<Move> &getMoveList(void) const;
    const Colour getNextMoveColour(void) const;
    Colour &getNextMoveColour(void);
    const int getEvaluation(void) const;
    const bool isInCheck(Colour kingsColour) const;
    std::vector<Move> getAllCandidateMoves(void);
    const BoardSquare & getSquare(std::string name) const;
    void printBoard(std::ostream &stream) const;
    const BoardSquare & operator[](const Locus &l) const;
    BoardSquare & operator[](const Locus &l);
    int perft(int depth);
private:
    Board(Colour nextMoveColour);
    void forEachPieceMoves(Colour c, moveCallback_t callback) const;
    bool isPieceUnderAttack(Locus l) const;
    std::vector<Locus> locatePiece(Colour c, PieceType t) const;
    std::vector<Move> moves_;
    Colour nextMoveColour_;
    ChessBoard board_;
};
