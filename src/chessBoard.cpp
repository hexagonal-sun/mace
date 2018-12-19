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

ChessBoard::orthogonal_ray_iterator
ChessBoard::getOrthogonalIterator(Locus l)
{
    return orthogonal_ray_iterator(*this, l);
}

ChessBoard::const_orthogonal_ray_iterator
ChessBoard::getOrthogonalIterator(Locus l) const
{
    return const_orthogonal_ray_iterator(*this, l);
}

ChessBoard::diagonal_ray_iterator
ChessBoard::getDiagonalIterator(Locus l)
{
    return diagonal_ray_iterator(*this, l);
}

ChessBoard::const_diagonal_ray_iterator
ChessBoard::getDiagonalIterator(Locus l) const
{
    return const_diagonal_ray_iterator(*this, l);
}

ChessBoard::knight_ray_iterator
ChessBoard::getKnightIterator(Locus l)
{
    return knight_ray_iterator(*this, l);
}

ChessBoard::const_knight_ray_iterator
ChessBoard::getKnightIterator(Locus l) const
{
    return const_knight_ray_iterator(*this, l);
}
