#include <list>
#include <stack>
#include <array>

#include "boardSquare.h"
#include "locus.h"
#include "movementTypes.hpp"

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

    template <typename ret_type,
              typename chessboard_type>
    class RayIterator
    {
        friend class ChessBoard;
    public:
        ret_type& operator*()
            {
                return cb_[curLoc_];
            }

        ret_type& operator->()
            {
                return cb_[curLoc_];
            }

        int getDistance(void) const
            {
                return manhattanDistance_;
            }

        void operator++()
            {
                curLoc_ += dir_;
            }
        bool operator==(const ChessBoard::RayIterator<ret_type,
                        chessboard_type> &other)
            {
                return !curLoc_.isValid();
            }

        bool operator!=(const ChessBoard::RayIterator<ret_type,
                        chessboard_type> &other)
            {
                return curLoc_.isValid();
            }

        ChessBoard::RayIterator<ret_type, chessboard_type> end()
            {
                return ChessBoard::RayIterator<ret_type,
                                               chessboard_type>
                    (cb_, Locus(), dir_);
            }

        ChessBoard::RayIterator<ret_type, chessboard_type> begin()
            {
                return *this;
            }
    private:
        RayIterator(chessboard_type &b, Locus l, Direction d)
            : curLoc_(l),
              cb_(b),
              manhattanDistance_(0),
              dir_(d)
            {
                curLoc_ += d;
            };

        Direction dir_;
        Locus curLoc_;
        chessboard_type &cb_;
        uint8_t manhattanDistance_;
    };

    typedef ChessBoard::RayIterator<BoardSquare,
                                    ChessBoard> ray_iterator;
    typedef ChessBoard::RayIterator<BoardSquare const,
                                    ChessBoard const> const_ray_iterator;

    ChessBoard::iterator<BoardSquare> begin();
    ChessBoard::iterator<BoardSquare> end() ;
    ChessBoard::iterator<BoardSquare const> begin() const;
    ChessBoard::iterator<BoardSquare const> end() const;
    ChessBoard::iterator<BoardSquare const> cbegin() const;
    ChessBoard::iterator<BoardSquare const> cend() const;
    ray_iterator getRayIterator(Locus l, Direction d);
    const_ray_iterator getRayIterator(Locus l, Direction d) const;


private:
    std::array<BoardSquare, 128> b_;
};
