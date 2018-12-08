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
    : board_(b), nextMoveColour_(nextMoveColour), evaluation_(0)
{
    evaluatePosition();
}

const BoardSquare & Board::getSquare(std::string name) const
{
    if (name.length() != 2)
        throw std::invalid_argument("square specifier of incorrect format");

    char rank = name[1];
    char file = name[0];

    return board_.at(Locus(rank, file));
}

Board Board::move(const BoardSquare &from, const BoardSquare &to) const
{
    // Some basic sanity checks here...
    if (!from.isOccupied())
        throw std::invalid_argument("Attempted to move from an empty square");

    auto piece = from.getPiece();

    if (piece->getColour() != nextMoveColour_)
        throw std::invalid_argument("Attempted to move the opposite turn's piece.");

    board_t newBoard = board_;

    const auto fromLoc = from.getLocus();
    const auto toLoc = to.getLocus();
    newBoard.at(toLoc).setPiece(newBoard.at(fromLoc).getPiece());
    newBoard.at(fromLoc).setEmpty();

    return Board(newBoard, getOppositeColour(nextMoveColour_));
}

Board Board::move(std::string from, std::string to) const
{
    return move(getSquare(from), getSquare(to));
}

Board Board::move(Locus from, Locus to) const
{
    return move(board_.at(from), board_.at(to));
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
    stream << "Evaluation: " << evaluation_ << "\n";
}

void Board::evaluatePosition(void)
{
    for (const auto &posSquare : board_) {
        auto square = posSquare.second;
        if (square.isOccupied())
            evaluation_ += square.getPiece()->getValue();
    }
}

bool Board::canMoveToSquare(Locus l, Colour c) const
{
    const auto &sq = board_.at(l);

    if (!sq.isOccupied())
        return true;

    // We can take opposite coloured pieces.
    if (sq.getPiece()->getColour() != c)
        return true;

    return false;
}

const BoardSquare & Board::operator[](const Locus &l) const
{
    return board_.at(l);
}

std::vector<Board> Board::getAllCandidateMoves(void) const
{
    std::vector<Board> ret;

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
