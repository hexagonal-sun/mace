#include <limits>

#include "board.h"
#include "mover.hpp"
#include "evaluate.h"
#include "moveStack.hpp"
#include "search.h"

enum class SearchType
{
    ALPHABETA,
    QUIESCENT
};

static int getPlayersEvaluation(Board &node)
{
    auto eval = getEvaluation(node);

    if (node.getNextMoveColour() == Colour::BLACK)
        eval = -eval;

    return eval;
}

template<SearchType type>
static int search(Board &node, size_t depth,
                     int alpha, int beta,
                     SearchResults &res)

{
    res.vistedNode();

    if (type == SearchType::ALPHABETA && depth == 0)
        return search<SearchType::QUIESCENT>(node, depth,
                                             alpha, beta, res);

    auto moves = MoveGen::getLegalMoves(node);

    if (type == SearchType::QUIESCENT)
        moves.erase(std::remove_if(moves.begin(),
                                   moves.end(),
                                   [&](const Move &move)
    {
        return !(move.getType() == MoveType::TAKE ||
                 move.getType() == MoveType::ENPASSANT_TAKE);
    }), moves.end());

    if (moves.size() == 0) {
        if (type == SearchType::QUIESCENT)
            return getPlayersEvaluation(node);

        if (node.isInCheck(node.getNextMoveColour()))
            return std::numeric_limits<int>::min() +
                ((res.getDepth() - depth) + 1);
        else
            return 0;
    }

    int val = std::numeric_limits<int>::min();

    if (type == SearchType::QUIESCENT) {
        val = getPlayersEvaluation(node);

        alpha = std::max(alpha, val);

        if (alpha >= beta)
            return val;
    }

    for (const auto move : moves) {
        Mover<MoverType::REVERT> m(move, node);

        val = std::max(val, -search<type>(node, depth - 1,
                                          -beta, -alpha, res));

        if (type == SearchType::ALPHABETA &&
            depth == res.getDepth() &&
            val > alpha)
        {
            res.setBestMove(move);
            res.setScore(val);
        }

        alpha = std::max(alpha, val);

        if (alpha >= beta)
            break;
    }

    return val;
}

static void doSearch(Board &b, SearchResults &results)
{
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    search<SearchType::ALPHABETA>(b, results.getDepth(),
                                     alpha, beta, results);
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
