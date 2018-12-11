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

Board::Board(board_t b, Colour nextMoveColour)
    : board_(b), nextMoveColour_(nextMoveColour)
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

bool Board::validateMove(std::string from, std::string to) const
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

const std::vector<Move> &Board::getMoveList(void) const
{
    return moves_;
}

bool Board::validateMove(const Locus & from, const Locus &to) const
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

bool Board::operator==(const Board &other) const
{
    return board_ == other.board_;
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

std::vector<Locus> Board::locatePiece(Colour c, PieceType t) const
{
    std::vector<Locus> ret;

    for (const auto &posSquare : board_) {
        const auto &square = posSquare.second;

        if (square.isOccupied() &&
            square.getPiece()->getColour() == c &&
            square.getPiece()->getPieceType() == t)
            ret.push_back(posSquare.first);
    }

    return ret;
}

bool Board::isPieceUnderAttack(Locus l) const
{
    const auto &pieceColour = board_.at(l).getPiece()->getColour();

    for (const auto &posSquare : board_)
        if (posSquare.second.isOccupied() &&
            posSquare.second.getPiece()->getColour() == getOppositeColour(pieceColour))
            for (const auto move : posSquare.second.getPiece()->getCandidateMoves(*this,
                                                                                  posSquare.first))
                if (std::get<1>(move) == l)
                    return true;

    return false;
}


const int Board::getEvaluation(void) const
{
    int eval = 0;

    for (const auto &posSquare : board_) {
        auto square = posSquare.second;
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
    return board_.at(l);
}

std::vector<Move> Board::getAllCandidateMoves(void) const
{
    std::vector<Move> ret;

    for (const auto &posSquare : board_) {
        auto square = posSquare.second;
        if (square.isOccupied() &&
            square.getPiece()->getColour() == nextMoveColour_) {
            auto pieceMoves = square.getPiece()->getCandidateMoves(*this,
                                                                   posSquare.first);
            ret.insert(ret.end(), pieceMoves.begin(), pieceMoves.end());
        }
    }

    return ret;
}

board_t Board::getEmptyBoard()
{
    board_t ret;
    for (const auto rank : RANKS)
        for (const auto file : FILES)
            ret.insert({Locus(rank, file), BoardSquare(rank, file)});

    return ret;
}

Board Board::getStartingBoard()
{
    board_t board = getEmptyBoard();

    for (const auto file : FILES) {
        board.at(Locus(Rank::SEVEN, file)).setPiece(std::make_shared<Pawn>(Pawn(Colour::BLACK)));
        board.at(Locus(Rank::TWO, file)).setPiece(std::make_shared<Pawn>(Pawn(Colour::WHITE)));
    }

    for (const auto col : {Colour::WHITE, Colour::BLACK}) {
        auto rank = col == Colour::WHITE ? Rank::ONE : Rank::EIGHT;
        for (const auto file : {File::A, File::H})
            board.at(Locus(rank, file)).setPiece(std::make_shared<Rook>(Rook(col)));

        for (const auto file : {File::B, File::G})
            board.at(Locus(rank, file)).setPiece(std::make_shared<Knight>(Knight(col)));

        for (const auto file : {File::C, File::F})
            board.at(Locus(rank, file)).setPiece(std::make_shared<Bishop>(Bishop(col)));

        board.at(Locus(rank, File::D)).setPiece(std::make_shared<Queen>(Queen(col)));
        board.at(Locus(rank, File::E)).setPiece(std::make_shared<King>(King(col)));
    }

    return Board(board, Colour::WHITE);
}
