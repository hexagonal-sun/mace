#include <thread>
#include <limits>
#include <atomic>

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

static std::atomic<bool> stopSearch(false);
static Move bestMove;

template<SearchType type>
static int search(Board &node, size_t depth,
                  int alpha, int beta, size_t ply,
                  SearchResults &res)

{
    if (stopSearch)
        return 0;

    res.vistedNode();

    if (type == SearchType::ALPHABETA && depth == 0)
        return search<SearchType::QUIESCENT>(node, depth,
                                             alpha, beta, ply + 1, res);

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
            return std::numeric_limits<int>::min() + 1 + ply;
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
                                          -beta, -alpha, ply + 1, res));

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
                                  alpha, beta, 0, results);
}

static void doDeepeningSearch(Board &b,
                              std::function<void(SearchResults &results)> dumpResults)
{
    const auto searchDirection = b.getNextMoveColour() == Colour::WHITE ?
        MinMax::MAX : MinMax::MIN;
    size_t depth = 1;

    while (1)
    {
        SearchResults results(depth, searchDirection);

        doSearch(b, results);
        results.finishedSearch();
        dumpResults(results);

        if (stopSearch)
            return;

        bestMove = results.getBestMove();
        depth++;
    }
}

Move searchMove(Board &b, const Clock::duration timeLimit,
                std::function<void(SearchResults &results)> dumpResults)
{
    stopSearch = false;
    std::thread searchThread(doDeepeningSearch, std::ref(b), dumpResults);

    std::this_thread::sleep_for(timeLimit);

    stopSearch = true;
    searchThread.join();

    return bestMove;
}
