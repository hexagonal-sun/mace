#pragma once

#include <boost/container/small_vector.hpp>

#include "locus.h"
#include "squareState.hpp"
#include "move.hpp"

class Board;

typedef boost::container::small_vector<Move, 64> moveList_t;

namespace MoveGen
{
    moveList_t getLegalMoves(Board &);
}
