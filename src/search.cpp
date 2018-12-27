#include <climits>

#include "board.h"
#include "piece.h"
#include "mover.hpp"
#include "moveStack.hpp"
#include "search.h"

static moveList_t moveStack;

static int alphaBeta(Board &node, size_t depth,
                     int alpha, int beta,
                     SearchResults &res)

{
    res.vistedNode();

    if (depth == 0) {
        auto eval = node.getEvaluation();

        if (node.getNextMoveColour() == Colour::BLACK)
            eval = -eval;

        if (res.getSearchDir() == MinMax::MAX &&
            eval > res.getScore()) {
            res.setScore(eval);
            res.setPV(moveStack);
        }

        if (res.getSearchDir() == MinMax::MIN &&
            eval < res.getScore()) {
            res.setScore(eval);
            res.setPV(moveStack);
        }

        return eval;
    }

    const auto &moves = MoveGen::getLegalMoves(node);

    if (moves.size() == 0)
        return node.getNextMoveColour() == Colour::WHITE ? -1024 : 1024;

    int val = -INT_MAX;

    for (const auto move : moves) {
        Mover<MoverType::REVERT> m(move, node);
        MoveStack ms(moveStack, move);

        val = std::max(val, -alphaBeta(node, depth - 1,
                                       -beta, -alpha, res));
        alpha = std::max(alpha, val);
        beta = std::min(beta, val);

        if (alpha >= beta)
            break;
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
