#include <climits>

#include "board.h"
#include "piece.h"
#include "mover.hpp"
#include "moveStack.hpp"
#include "search.h"

static int alphaBeta(Board &node, size_t depth,
                     int alpha, int beta,
                     SearchResults &res)

{
    res.vistedNode();

    if (depth == 0) {
        auto eval = node.getEvaluation();

        if (node.getNextMoveColour() == Colour::BLACK)
            eval = -eval;

        return eval;
    }

    const auto &moves = MoveGen::getLegalMoves(node);

    if (moves.size() == 0) {
        if (node.isInCheck(node.getNextMoveColour()))
            return -1024;
        else
            return 0;
    }

    int val = -INT_MAX;

    for (const auto move : moves) {
        Mover<MoverType::REVERT> m(move, node);

        val = std::max(val, -alphaBeta(node, depth - 1,
                                       -beta, -alpha, res));

        if (depth == res.getDepth()) {
            if (val > alpha)
                res.setBestMove(move);
        }

        alpha = std::max(alpha, val);
        beta = std::min(beta, val);

        if (alpha >= beta)
            break;

        }
    }

    return val;
}

static void doSearch(Board &b, SearchResults &results)
{
    int alpha = -INT_MAX;
    int beta = INT_MAX;

    alphaBeta(b, results.getDepth(), alpha, beta, results);
}

Move searchMove(Board &b, const Clock::duration timeLimit,
                std::function<void(SearchResults &results)> dumpResults)
{
    const auto searchStart = Clock::now();
    const auto searchEnd = searchStart + timeLimit;
    const auto searchDirection = b.getNextMoveColour() == Colour::WHITE ?
        MinMax::MAX : MinMax::MIN;
    size_t depth = 1;

    while (1)
    {
        SearchResults results(depth, searchDirection);

        doSearch(b, results);
        results.finishedSearch();
        dumpResults(results);

        if (Clock::now() > searchEnd)
            return results.getBestMove();

        depth++;
    }
}
