
#include <array>

#include "boardSquare.h"
#include "locus.h"

enum class Ray
{
    ORTHOGONAL,
    DIAGONAL,
    KNIGHT
};

class ChessBoard
{
public:
    ChessBoard();
    const BoardSquare &operator[](const Locus &l) const;
    BoardSquare &operator[](const Locus &l);
    const BoardSquare &at(const Locus &l) const;

    template <typename T>
    class iterator
    {
    public:
        iterator(T *ptr)
            : curIdx_(0),
              rankCount_(0),
              ptr_(ptr)
            {};

        T &operator*()
            {
                return *ptr_;
            }

        T &operator->()
            {
                return *ptr_;
            }

        void operator++(void)
            {
                rankCount_++;

               if (rankCount_ == 8) {
                    ptr_ += 8;
                    rankCount_ = 0;
                }

               ptr_++;
            }

        bool operator==(const ChessBoard::iterator<T> &other)
            {
                return ptr_ == other.ptr_;
            }

        bool operator!=(const ChessBoard::iterator<T> &other)
            {
                return ptr_ != other.ptr_;
            }
    private:
        T *ptr_;
        int rankCount_;
        uint8_t curIdx_;
    };

    ChessBoard::iterator<BoardSquare> begin();
    ChessBoard::iterator<BoardSquare> end() ;
    ChessBoard::iterator<BoardSquare const> begin() const;
    ChessBoard::iterator<BoardSquare const> end() const;
    ChessBoard::iterator<BoardSquare const> cbegin() const;
    ChessBoard::iterator<BoardSquare const> cend() const;

private:
    std::array<BoardSquare, 128> b_;
};
