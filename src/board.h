#pragma once
#include <array>
#include <ostream>
#include <array>
#include <vector>

#include "chessBoard.h"
#include "move.hpp"
#include "moveGen.h"
#include "castlingRights.hpp"

enum class Colour;
enum class PieceType;
class Piece;

typedef boost::container::small_vector<Locus, 8> locusList_t;

class Board
{
public:
    static Board getStartingBoard(void);
    static Board constructFromFEN(std::string fen);
    Move validateMove(std::string from, std::string to);
    Move validateMove(const Locus &from, const Locus &to);
    const Colour getNextMoveColour(void) const;
    Colour &getNextMoveColour(void);
    const int getEvaluation(void) const;
    const bool isInCheck(Colour kingsColour) const;
    const Locus &getKingLocus(Colour c) const;
    Locus &getKingLocus(Colour c);
    const CastlingRights getCastlingRights(void) const;
    CastlingRights &getCastlingRights(void);
    const Locus &getEnPassantLocus(void) const;
    Locus &getEnPassantLocus(void);
    bool isDraw() const;
    ChessBoard &getChessBoard();
    const ChessBoard &getChessBoard() const;
    const SquareState & getSquare(std::string name) const;
    void printBoard(std::ostream &stream) const;
    void printFEN(std::ostream &os) const;
    const SquareState & operator[](const Locus &l) const;
    SquareState & operator[](const Locus &l);
    bool isSquareUnderAttack(Locus l, Colour c) const;
    int perft(int depth, bool divide);
    std::map<PieceType, int> pieceCounts;
private:
    Board(Colour nextMoveColour);
    CastlingRights castlingRights_;
    Locus enPassantCapture_;
    std::array<Locus, 2> kingLocus_;
    locusList_t locatePiece(Colour c, PieceType t) const;
    Colour nextMoveColour_;
    ChessBoard board_;
};
