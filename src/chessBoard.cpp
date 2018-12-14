#include "chessBoard.h"

ChessBoard::ChessBoard()
{
    for (const auto rank : RANKS)
        for (const auto file : FILES)
            b_[Locus(rank, file).getIndex()] = BoardSquare(Locus(rank, file));
}

const BoardSquare &
ChessBoard::operator[](const Locus &l) const
{
    return b_[l.getIndex()];
}

BoardSquare &
ChessBoard::operator[](const Locus &l)
{
    return b_[l.getIndex()];
}

const BoardSquare &
ChessBoard::at(const Locus &l) const
{
    return b_.at(l.getIndex());
}

ChessBoard::iterator<BoardSquare>
ChessBoard::begin()
{
    return ChessBoard::iterator<BoardSquare>(&b_[0]);
}

ChessBoard::iterator<BoardSquare>
ChessBoard::end()
{
    return ChessBoard::iterator<BoardSquare>(&b_[128]);
}

ChessBoard::iterator<BoardSquare const>
ChessBoard::begin() const
{
    return ChessBoard::iterator<BoardSquare const>(&b_[0]);
}

ChessBoard::iterator<BoardSquare const>
ChessBoard::end() const
{
    return ChessBoard::iterator<BoardSquare const>(&b_[128]);
}

ChessBoard::iterator<BoardSquare const>
ChessBoard::cbegin() const
{
    return ChessBoard::iterator<BoardSquare const>(&b_[0]);
}

ChessBoard::iterator<BoardSquare const>
ChessBoard::cend() const
{
    return ChessBoard::iterator<BoardSquare const>(&b_[128]);
}
