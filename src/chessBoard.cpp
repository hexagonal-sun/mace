#include "chessBoard.h"

const SquareState &
ChessBoard::operator[](const Locus &l) const
{
    return b_[l.getIndex()];
}

SquareState &
ChessBoard::operator[](const Locus &l)
{
    return b_[l.getIndex()];
}

ZobristHash
ChessBoard::getHash() const
{
    return hash_;
}

ZobristHash &
ChessBoard::getHash()
{
    return hash_;
}

const SquareState &
ChessBoard::at(const Locus &l) const
{
    return b_.at(l.getIndex());
}

ChessBoard::iterator<SquareState>
ChessBoard::begin()
{
    return ChessBoard::iterator<SquareState>(&b_[0]);
}

ChessBoard::iterator<SquareState>
ChessBoard::end()
{
    return ChessBoard::iterator<SquareState>(&b_[128]);
}

ChessBoard::iterator<SquareState const>
ChessBoard::begin() const
{
    return ChessBoard::iterator<SquareState const>(&b_[0]);
}

ChessBoard::iterator<SquareState const>
ChessBoard::end() const
{
    return ChessBoard::iterator<SquareState const>(&b_[128]);
}

ChessBoard::iterator<SquareState const>
ChessBoard::cbegin() const
{
    return ChessBoard::iterator<SquareState const>(&b_[0]);
}

ChessBoard::iterator<SquareState const>
ChessBoard::cend() const
{
    return ChessBoard::iterator<SquareState const>(&b_[128]);
}

ChessBoard::ray_iterator
ChessBoard::getRayIterator(Locus l, Direction d)
{
    return ray_iterator(*this, l, d);
}

ChessBoard::const_ray_iterator
ChessBoard::getRayIterator(Locus l, Direction d) const
{
    return const_ray_iterator(*this, l, d);
}
