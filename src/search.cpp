#include <iostream>
#include <climits>

#include "mover.hpp"
#include "search.h"
#include "piece.h"

static int alphaBeta(Board &node, size_t depth,
                     int alpha, int beta,
                     bool maximizePlayer,
                     Move *bestMove)

{
    if (depth == 0)
        return node.getEvaluation();

    if (maximizePlayer) {
        int val = -INT_MAX;

        for (const auto move : node.getAllCandidateMoves()) {
            Mover m(move, node);
            int prevAlpha = alpha;

            val = std::max(val, alphaBeta(node, depth - 1,
                                          alpha, beta, false, nullptr));
            alpha = std::max(alpha, val);

            if (bestMove && alpha != prevAlpha)
                *bestMove = move;

            if (alpha >= beta)
                break;
        }
        return val;
    } else {
        int val = INT_MAX;

        for (const auto move : node.getAllCandidateMoves()) {
            Mover m(move, node);
            int prevBeta = beta;

            val = std::min(val, alphaBeta(node, depth - 1,
                                          alpha, beta, true, nullptr));
            beta = std::min(beta, val);

            if (bestMove && beta != prevBeta)
                *bestMove = move;

            if (alpha >= beta)
                break;
        }
        return val;
    }
}

Move searchMove(Board &b, size_t depth)
{
    int alpha = -INT_MAX;
    int beta = INT_MAX;

    Move m(Locus('1', 'a'), Locus('1', 'a'));

    int result = alphaBeta(b, depth, alpha, beta,
                           b.getNextMoveColour() == Colour::WHITE ? true : false,
                           &m);

    return m;
}
