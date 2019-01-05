#pragma once
#include <array>
#include <functional>
#include <unordered_set>
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
    Move validateMove(std::string from, std::string to,
                      const MoveType promotion);
    Move validateMove(const Locus &from, const Locus &to,
                      const MoveType promotion);
    const Colour getNextMoveColour(void) const;
    Colour &getNextMoveColour(void);
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
    SquareStateModifier operator[](const Locus &l);
    bool isSquareUnderAttack(Locus l, Colour c) const;
    int perft(int depth, bool divide,
              std::function<void(Move)> moveCallback = nullptr,
              std::function<void(Move)> unmoveCallback = nullptr);
    std::map<PieceType, int> pieceCounts;

    const ZobristHash getHash() const { return board_.getHash(); }
    ZobristHash &getHash() { return board_.getHash(); }

    void pushPosition() { seenPositions_.insert(getHash()); }
    void popPosition() { seenPositions_.erase(getHash()); }

private:
    Board(Colour nextMoveColour);
    CastlingRights castlingRights_;
    Locus enPassantCapture_;
    std::unordered_set<ZobristHash, Zobrist::Hash> seenPositions_;
    std::array<Locus, 2> kingLocus_;
    locusList_t locatePiece(Colour c, PieceType t) const;
    Colour nextMoveColour_;
    ChessBoard board_;
};
