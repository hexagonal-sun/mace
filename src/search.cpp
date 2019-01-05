#include <thread>
#include <limits>
#include <atomic>

#include "board.h"
#include "mover.hpp"
#include "evaluate.h"
#include "moveStack.hpp"
#include "search.h"

constexpr int INF = std::numeric_limits<int>::max();

static int getPlayersEvaluation(Board &node)
{
    auto eval = getEvaluation(node);

    if (node.getNextMoveColour() == Colour::BLACK)
        eval = -eval;

    return eval;
}

static std::atomic<bool> stopSearch(false);
static Move bestMove;

static int qsearch(Board &node, int alpha, int beta, int depth)
{

    if (stopSearch)
        return 0;

    int standPat =  getPlayersEvaluation(node);

    if( standPat >= beta )
        return beta;

    if( alpha < standPat )
        alpha = standPat;

    if (!depth)
        return alpha;

    auto moves = MoveGen::getLegalMoves(node);

    moves.erase(std::remove_if(moves.begin(),
                               moves.end(),
                               [&](const Move &move)
    {
        return !(move.getType() == MoveType::TAKE ||
                 move.getType() == MoveType::ENPASSANT_TAKE);
    }), moves.end());

    for (const auto move : moves) {
        Mover<MoverType::REVERT> m(move, node);

        if (node.seenPosition())
            // If we have already seen this position, it's repetition.
            return 0;

        // Check for the 50 move rule.
        if (node.getHalfMoveClock() >= 100)
            return 0;

        node.pushPosition();

        int score = -qsearch(node, -beta, -alpha, depth -1);

        node.popPosition();

        if (score >= beta)
            return beta;

        if (score > alpha)
            alpha = score;
    }

    return alpha;
}

static int absearch(Board &node, size_t depth,
                    int alpha, int beta, size_t ply,
                    SearchResults &res)

{
    if (stopSearch)
        return 0;

    res.vistedNode();

    if (depth == 0)
        return qsearch(node, alpha, beta, res.getDepth() * 2);

    auto moves = MoveGen::getLegalMoves(node);

    if (moves.size() == 0) {
        if (node.isInCheck(node.getNextMoveColour()))
            return (-INF) + 1 + ply;
        else
            return 0;
    }

    int val = -INF;

    for (const auto move : moves) {
        Mover<MoverType::REVERT> m(move, node);

        val = std::max(val, -absearch(node, depth - 1,
                                      -beta, -alpha, ply + 1, res));

        if (depth == res.getDepth() && val > alpha)
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
    int alpha = -INF;
    int beta = INF;

    absearch(b, results.getDepth(), alpha, beta, 0, results);
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
