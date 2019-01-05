#include <tuple>
#include <list>
#include <stack>
#include <array>

#include "colour.hpp"
#include "locus.h"
#include "pieceType.hpp"
#include "squareState.hpp"
#include "zobrist.h"


class ChessBoard
{
public:
    const SquareState &operator[](const Locus &l) const;
    SquareState &operator[](const Locus &l);
    const SquareState &at(const Locus &l) const;
    ZobristHash getHash() const;
    ZobristHash &getHash();

    ChessBoard()
        : hash_(0) {}

    template <typename T>
    class iterator
    {
    public:
        iterator(T *ptr)
            : curIdx_(0),
              rankCount_(0),
              ptr_(ptr)
            {};

        std::tuple<T, Locus> operator*()
            {
                return std::tuple<T, Locus>(*ptr_, Locus(curIdx_));
            }

        std::tuple<T, Locus> operator->()
            {
                return std::tuple<T, Locus>(*ptr_, Locus(curIdx_));
            }

        void operator++(void)
            {
                rankCount_++;

               if (rankCount_ == 8) {
                    ptr_ += 8;
                    curIdx_ += 8;
                    rankCount_ = 0;
                }

               ptr_++;
               curIdx_++;
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
        uint8_t curIdx_;
        int rankCount_;
        T *ptr_;
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

        Locus curLoc_;
        chessboard_type &cb_;
        uint8_t manhattanDistance_;
        Direction dir_;
    };

    typedef ChessBoard::RayIterator<SquareState,
                                    ChessBoard> ray_iterator;
    typedef ChessBoard::RayIterator<SquareState const,
                                    ChessBoard const> const_ray_iterator;

    ChessBoard::iterator<SquareState> begin();
    ChessBoard::iterator<SquareState> end() ;
    ChessBoard::iterator<SquareState const> begin() const;
    ChessBoard::iterator<SquareState const> end() const;
    ChessBoard::iterator<SquareState const> cbegin() const;
    ChessBoard::iterator<SquareState const> cend() const;
    ray_iterator getRayIterator(Locus l, Direction d);
    const_ray_iterator getRayIterator(Locus l, Direction d) const;


private:
    std::array<SquareState, 128> b_;
    ZobristHash hash_;
};
