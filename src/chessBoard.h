
#include <array>

#include "boardSquare.h"
#include "locus.h"

class ChessBoard
{
public:
    ChessBoard();
    const BoardSquare &operator[](const Locus &l) const;
    BoardSquare &operator[](const Locus &l);
    const BoardSquare &at(const Locus &l) const;
    std::array<BoardSquare, 64>::iterator begin() { return b_.begin(); };
    std::array<BoardSquare, 64>::iterator end() { return b_.end(); };
    std::array<BoardSquare, 64>::const_iterator begin() const { return b_.cbegin(); };
    std::array<BoardSquare, 64>::const_iterator end() const { return b_.cend(); };
    std::array<BoardSquare, 64>::const_iterator cbegin() const { return b_.cbegin(); };
    std::array<BoardSquare, 64>::const_iterator cend() const { return b_.cend(); };
private:
    std::array<BoardSquare, 64> b_;
};
