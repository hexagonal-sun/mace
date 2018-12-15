#include <algorithm>
#include <locale>
#include <iostream>
#include "board.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "queen.h"
#include "king.h"
#include "bishop.h"
#include "boardSquare.h"
#include "mover.hpp"

Board::Board(Colour nextMoveColour)
    : nextMoveColour_(nextMoveColour)
{
}

const BoardSquare & Board::getSquare(std::string name) const
{
    if (name.length() != 2)
        throw std::invalid_argument("square specifier of incorrect format");

    char rank = name[1];
    char file = name[0];

    return board_.at(Locus(rank, file));
}

bool Board::validateMove(std::string from, std::string to)
{
    return validateMove(getSquare(from).getLocus(), getSquare(to).getLocus());
}

const Colour Board::getNextMoveColour(void) const
{
    return nextMoveColour_;
}

Colour & Board::getNextMoveColour(void)
{
    return nextMoveColour_;
}

bool Board::validateMove(const Locus & from, const Locus &to)
{
    auto candidate = std::make_tuple(from, to);

    const auto &allowedMoves = getAllCandidateMoves();

    if (std::find(allowedMoves.begin(),
                  allowedMoves.end(),
                  candidate) == allowedMoves.end())
        return false;
    else
        return true;
}

void Board::printBoard(std::ostream &stream) const
{
    for (const auto rank : RANKS)
    {
        for (const auto file : FILES) {
            board_.at(Locus(rank, file)).printSquare(stream);

            stream << " ";
        }

        stream << "\n";
    }

    stream << "To move: " << colourNames.at(nextMoveColour_) << "\n";
}

locusList_t Board::locatePiece(Colour c, PieceType t) const
{
    locusList_t ret;

    for (const auto &square : board_) {
        if (square.isOccupied() &&
            square.getPiece()->getColour() == c &&
            square.getPiece()->getPieceType() == t)
            ret.push_back(square.getLocus());
    }

    return ret;
}

bool Board::isPieceUnderAttack(Locus l) const
{
    const auto &pieceColour = board_.at(l).getPiece()->getColour();

    bool ret = false;

    forEachPieceMoves(getOppositeColour(pieceColour),
                      [&](Piece *piece,
                          const moveList_t &moves)
    {
        for (const auto &move : moves)
            if (std::get<1>(move) == l) {
                ret = true;
                return false;
            }
        return true;
    });

    return ret;
}

const bool Board::isInCheck(Colour kingsColour) const
{
    const auto &kingLocus = getKingLocus(kingsColour);

    return isPieceUnderAttack(kingLocus);
}


const int Board::getEvaluation(void) const
{
    int eval = 0;

    for (const auto &square : board_) {
        if (square.isOccupied())
            eval += square.getPiece()->getValue();
    }

    return eval;
}

const BoardSquare & Board::operator[](const Locus &l) const
{
    return board_.at(l);
}

BoardSquare & Board::operator[](const Locus &l)
{
    return board_[l];
}

moveList_t Board::getAllCandidateMoves(void)
{
    moveList_t ret;

    const auto nonCheckCallback = [&](Piece *piece,
                                      const moveList_t &moves)
    {
        auto ourColour = getNextMoveColour();

        for (const auto &move : moves) {
            Mover m(move, *this);
            if (!isInCheck(ourColour))
                ret.push_back(move);
        }

        return true;
    };

    const auto checkCallback = [&](Piece *piece,
                                   const moveList_t &moves)
    {
        auto ourColour = getNextMoveColour();

        for (const auto &move : moves) {
            Mover m(move, *this);
            if (!isInCheck(ourColour))
                ret.push_back(move);
        }

        return true;
    };

    if (isInCheck(getNextMoveColour()))
        forEachPieceMoves(getNextMoveColour(), checkCallback);
    else
        forEachPieceMoves(getNextMoveColour(), nonCheckCallback);

    return ret;
}

const Locus &Board::getKingLocus(Colour c) const
{
    return kingLocus_[static_cast<int>(c)];
}

Locus &Board::getKingLocus(Colour c)
{
    return kingLocus_[static_cast<int>(c)];
}

int Board::perft(int depth)
{
    if (!depth)
        return 1;

    int nodes = 0;

    for (const auto move : getAllCandidateMoves()) {
        Mover m(move, *this);

        nodes += perft(depth - 1);
    }

    return nodes;
}

void Board::forEachPieceMoves(Colour c, moveCallback_t callback) const
{
    for (const auto &square : board_) {
        if (square.isOccupied() &&
            square.getPiece()->getColour() == c) {
            const auto pieceMoves = square.getPiece()->getCandidateMoves(*this,
                                                                         square.getLocus());
            if (!callback(square.getPiece(), pieceMoves))
                return;
        }
    }
}


Board Board::getStartingBoard()
{
    Board board(Colour::WHITE);

    for (const auto file : FILES) {
        board[Locus(Rank::SEVEN, file)].setPiece(new Pawn(Colour::BLACK));
        board[Locus(Rank::TWO, file)].setPiece(new Pawn(Colour::WHITE));
    }

    for (const auto col : {Colour::WHITE, Colour::BLACK}) {
        auto rank = col == Colour::WHITE ? Rank::ONE : Rank::EIGHT;
        for (const auto file : {File::A, File::H})
            board[Locus(rank, file)].setPiece(new Rook(col));

        for (const auto file : {File::B, File::G})
            board[Locus(rank, file)].setPiece(new Knight(col));

        for (const auto file : {File::C, File::F})
            board[Locus(rank, file)].setPiece(new Bishop(col));

        board[Locus(rank, File::D)].setPiece(new Queen(col));
        board[Locus(rank, File::E)].setPiece(new King(col));
        board.getKingLocus(col) = Locus(rank, File::E);
    }

    return board;
}
