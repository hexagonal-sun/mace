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
