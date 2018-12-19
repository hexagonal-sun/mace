#include <list>
#include <stack>
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

    template <typename ret_type,
              typename chessboard_type,
              Ray ray_type>
    class RayIterator
    {
        friend class ChessBoard;
    public:
        ret_type &operator*()
            {
                return cb_[curLoc_];
            }

        ret_type &operator->()
            {
                return cb_[curLoc_];
            }

        void operator++()
            {
                if (ray_type == Ray::KNIGHT)
                    curLoc_ = originalLoc_;

            nextDir:;
                if (dirs_.empty()) {
                    curLoc_ = Locus();
                    return;
                }

                for (auto dir : dirs_.top()) {
                    curLoc_ = curLoc_.translate(dir);

                    if (!curLoc_.isValid()) {

                        dirs_.pop();
                        curLoc_ = originalLoc_;

                        goto nextDir;
                    }
                }

                // For knight rays, we only want to apply the
                // transformation once.
                if (ray_type == Ray::KNIGHT)
                    dirs_.pop();
            }
        bool operator==(const ChessBoard::RayIterator<ret_type,
                                                      chessboard_type,
                                                      ray_type> &other)
            {
                return curLoc_ == other.curLoc_;
            }

        bool operator!=(const ChessBoard::RayIterator<ret_type,
                                                      chessboard_type,
                                                      ray_type> &other)
            {
                return (!(curLoc_ == other.curLoc_));
            }

        ChessBoard::RayIterator<ret_type, chessboard_type, ray_type> &begin()
            {
                return *this;
            }

        ChessBoard::RayIterator<ret_type, chessboard_type, ray_type> end()
            {
                return ChessBoard::RayIterator<ret_type,
                                               chessboard_type,
                                               ray_type>(cb_,
                                                         Locus());
            }
    private:
        RayIterator(chessboard_type &b, Locus l)
            : curLoc_(l),
              cb_(b),
              originalLoc_(l)
            {
                switch (ray_type) {
                case Ray::ORTHOGONAL:
                    dirs_ = std::stack<std::list<Direction>>({
                            {Direction::NORTH},
                            {Direction::EAST},
                            {Direction::SOUTH},
                            {Direction::WEST}
                        });
                    break;
                case Ray::DIAGONAL:
                    dirs_ = std::stack<std::list<Direction>>({
                            {Direction::NORTH, Direction::EAST},
                            {Direction::NORTH, Direction::WEST},
                            {Direction::SOUTH, Direction::EAST},
                            {Direction::SOUTH, Direction::WEST}
                        });
                    break;
                case Ray::KNIGHT:
                    dirs_ = std::stack<std::list<Direction>>({
                            {Direction::NORTH, Direction::NORTH, Direction::EAST},
                            {Direction::NORTH, Direction::NORTH, Direction::WEST},
                            {Direction::EAST, Direction::EAST, Direction::NORTH},
                            {Direction::EAST, Direction::EAST, Direction::SOUTH},
                            {Direction::SOUTH, Direction::SOUTH, Direction::EAST},
                            {Direction::SOUTH, Direction::SOUTH, Direction::WEST},
                            {Direction::WEST, Direction::WEST, Direction::NORTH},
                            {Direction::WEST, Direction::WEST, Direction::SOUTH}
                        });
                    break;
                default:
                    throw std::invalid_argument("Unknown ray type");
                }
            };
        const Locus originalLoc_;
        Ray rayType_;
        Locus curLoc_;
        chessboard_type &cb_;
        std::stack<std::list<Direction>> dirs_;
    };

    typedef ChessBoard::RayIterator<BoardSquare,
                                    ChessBoard,
                                    Ray::ORTHOGONAL> orthogonal_ray_iterator;
    typedef ChessBoard::RayIterator<BoardSquare const,
                                    ChessBoard const,
                                    Ray::ORTHOGONAL> const_orthogonal_ray_iterator;

    typedef ChessBoard::RayIterator<BoardSquare,
                                    ChessBoard,
                                    Ray::DIAGONAL> diagonal_ray_iterator;
    typedef ChessBoard::RayIterator<BoardSquare const,
                                    ChessBoard const,
                                    Ray::DIAGONAL> const_diagonal_ray_iterator;

    ChessBoard::iterator<BoardSquare> begin();
    ChessBoard::iterator<BoardSquare> end() ;
    ChessBoard::iterator<BoardSquare const> begin() const;
    ChessBoard::iterator<BoardSquare const> end() const;
    ChessBoard::iterator<BoardSquare const> cbegin() const;
    ChessBoard::iterator<BoardSquare const> cend() const;
    orthogonal_ray_iterator getOrthogonalIterator(Locus l);
    const_orthogonal_ray_iterator getOrthogonalIterator(Locus l) const;
    diagonal_ray_iterator getDiagonalIterator(Locus l);
    const_diagonal_ray_iterator getDiagonalIterator(Locus l) const;

private:
    std::array<BoardSquare, 128> b_;
};
