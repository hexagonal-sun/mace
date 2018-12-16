#pragma once

#include <boost/container/small_vector.hpp>
#include "locus.h"
#include "piece.h"

enum class MoveType
{
    UNOCCUPIED,
    CASTLE,
    ENPASSANT_TAKE,
    ENPASSANT_ADVANCE,
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
    bool isValid_;
    Locus from_;
    Locus to_;
    MoveType type_;
};

static inline std::ostream &operator<<(std::ostream &os, const Move &l)
{
    os << l.getFrom() << l.getTo();

    return os;
}

typedef boost::container::small_vector<Move, 64> moveList_t;