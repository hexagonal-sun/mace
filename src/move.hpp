#pragma once

#include <boost/container/small_vector.hpp>
#include "locus.h"

enum class MoveType
{
    UNOCCUPIED,
    CASTLE_QUEENSIDE,
    CASTLE_KINGSIDE,
    ENPASSANT_TAKE,
    ENPASSANT_ADVANCE,
    PROMOTE_QUEEN,
    PROMOTE_KNIGHT,
    PROMOTE_BISHOP,
    PROMOTE_ROOK,
    TAKE
};

class Move
{
public:
    Move()
        : isValid_(false) {};

    Move(Locus from, Locus to, MoveType t)
        : from_(from), to_(to), type_(t), isValid_(true) {};

    const Locus &getFrom() const { return from_; };
    const Locus &getTo() const { return to_; };
    const MoveType getType() const { return type_; };
    const bool isValid() const { return isValid_; };

private:
    Locus from_;
    Locus to_;
    MoveType type_;
    bool isValid_;
};

static inline std::ostream &operator<<(std::ostream &os, const Move &l)
{
    os << l.getFrom() << l.getTo();

    switch (l.getType())
    {
        case MoveType::PROMOTE_QUEEN:
            os << "q";
            break;
        case MoveType::PROMOTE_ROOK:
            os << "r";
            break;
        case MoveType::PROMOTE_BISHOP:
            os << "b";
            break;
        case MoveType::PROMOTE_KNIGHT:
            os << "n";
            break;
        default:
            break;
    }

    return os;
}

typedef boost::container::small_vector<Move, 64> moveList_t;
