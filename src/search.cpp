#include <iostream>
#include <climits>

#include "board.h"
#include "piece.h"
#include "mover.hpp"
#include "search.h"

enum class MinMax {
    MIN,
    MAX
};


template <MinMax type>
static int alphaBeta(Board &node, size_t depth,
                     int alpha, int beta,
                     Move *bestMove)

{
    if (depth == 0)
        return node.getEvaluation();

    const auto &moves = MoveGen::getLegalMoves(node);

    if (moves.size() == 0)
        return node.getNextMoveColour() == Colour::WHITE ? -1024 : 1024;

    int val = (type == MinMax::MAX ? -INT_MAX : INT_MAX);

    for (const auto move : moves) {
        Mover<MoverType::REVERT> m(move, node);
        int prevVal = val;

        if (type == MinMax::MAX) {
            val = std::max(val, alphaBeta<MinMax::MIN>(node, depth - 1,
                                                       alpha, beta, nullptr));
            alpha = std::max(alpha, val);
        } else {
            val = std::min(val, alphaBeta<MinMax::MAX>(node, depth - 1,
                                                       alpha, beta, nullptr));
            beta = std::min(beta, val);
        }

        if (bestMove && prevVal != val)
            *bestMove = move;

        if (alpha >= beta)
            break;
    }

    return val;
}

Move searchMove(Board &b, size_t depth)
{
    int alpha = -INT_MAX;
    int beta = INT_MAX;
    Move m;

    auto searchFunc = b.getNextMoveColour() == Colour::WHITE ?
        alphaBeta<MinMax::MAX> : alphaBeta<MinMax::MIN>;

    searchFunc(b, depth, alpha, beta, &m);

    return m;
}
