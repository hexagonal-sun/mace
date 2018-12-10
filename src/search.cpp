#include <iostream>
#include <climits>

#include "search.h"
#include "piece.h"

typedef std::tuple<int, std::shared_ptr<Board>> bval;

static bval alphaBeta(const Board &node, size_t depth,
                      bval alpha, bval beta,
                      bool maximizePlayer)
{
    if (depth == 0)
        return std::make_tuple(node.getEvaluation(),
                               std::make_shared<Board>(node));

    if (maximizePlayer) {
        bval val = std::make_tuple(-INT_MAX, nullptr);
        for (const auto &child : node.getAllCandidateMoves()) {
            val = std::max(val, alphaBeta(child, depth - 1,
                                          alpha, beta, false));
            alpha = std::max(alpha, val);
            if (std::get<0>(alpha) >= std::get<0>(beta))
                break;
        }
        return val;
    } else {
        bval val = std::make_tuple(INT_MAX, nullptr);
        for (const auto &child : node.getAllCandidateMoves()) {
            val = std::min(val, alphaBeta(child, depth - 1,
                                          alpha, beta, true));
            beta = std::min(beta, val);
            if (std::get<0>(alpha) >= std::get<0>(beta))
                break;
        }
        return val;
    }
}

Move searchMove(const Board &b, size_t depth)
{
    size_t moveNumber = b.getMoveList().size();

    bval alpha = std::make_tuple(-INT_MAX, nullptr);
    bval beta = std::make_tuple(INT_MAX, nullptr);

    bval result = alphaBeta(b, depth, alpha, beta,
                            b.getNextMoveColour() == Colour::WHITE ? true : false);

    return std::get<1>(result)->getMoveList().at(moveNumber);
}
