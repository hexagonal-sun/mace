#include <algorithm>
#include <random>
#include <fstream>
#include "mover.hpp"
#include "mcts.h"

static size_t totalSimulations = 0;

Move MCTS::selectMove(size_t numIterations, Board &b)
{
    totalSimulations = 0;

    while (numIterations--)
    {
        root_.MCTS(b);
        totalSimulations++;
    }

    // When selecting the best move from the root node, we don't want
    // to use the UCB function since we don't want to take exploration
    // into account.  We want to choose the node that has the highest
    // expected outcome.
    return root_.selectBestMove();
}

void
MCTS::dumpTree(std::string filename) const
{
    std::ofstream outFile(filename);
    outFile << "digraph MCTS {\n";
    root_.dumpNode(outFile);
    outFile << "}\n";
}

MCTS::MCTSNode::MCTSNode(Board &b, Colour c, Move m, size_t depth,
                         size_t id)
    : ourColour_(c), move_(m), visits_(0), wins_(0), depth_(depth),
      id_(id)
{
    Mover<MoverType::REVERT> mover(m, b);

    unexploredMoves_ = MoveGen::getLegalMoves(b);
}

MCTS::MCTSNode::MCTSNode(Board &b)
    : ourColour_(b.getNextMoveColour()),
      visits_(0), wins_(0), depth_(0), id_(0)
{
    unexploredMoves_ = MoveGen::getLegalMoves(b);
}

Move
MCTS::MCTSNode::getMove() const
{
    return move_;
}

void MCTS::MCTSNode::dumpNode(std::ostream &os) const
{
    os << "    n" << id_ << " [label = \"" << move_
       << "\\n" << wins_ << " / " << visits_ <<"\"]\n";

    for (const auto &node : children_) {
        node.dumpNode(os);
        os << "    n" << id_ << " -> n" << node.getId() << "\n";
    }
}

size_t
MCTS::MCTSNode::getId() const
{
    return id_;
}

void
MCTS::MCTSNode::updateStats(bool didWin)
{
    visits_++;

    if (didWin)
        wins_++;
}

bool
MCTS::MCTSNode::rollout(Move move, Board &b) const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    Mover<MoverType::REVERT> m(move, b);

    if (b.isDraw())
        return false;

    auto candidateMoves = MoveGen::getLegalMoves(b);

    if (candidateMoves.empty())
        return b.getNextMoveColour() == ourColour_ ? false : true;

    auto upperBound = candidateMoves.size() - 1;

    std::uniform_int_distribution<> dis(0, upperBound);

    return rollout(candidateMoves[dis(gen)], b);
}

bool
MCTS::MCTSNode::doMCTS(Board &b)
{
    Mover<MoverType::REVERT> m(move_, b);

    return MCTS(b);
}

bool
MCTS::MCTSNode::MCTS(Board &b)
{
    // Phase 1: Descend.
    if (unexploredMoves_.empty()) {
        bool result = selectChild().doMCTS(b);

        updateStats(result);

        return result;
    }

    // Phase 2: Expand.
    auto &newNode = expandChild(b);

    // Phase 3: rollout.
    bool result = newNode.rollout(newNode.getMove(), b);

    // Phase 4: backprop.
    newNode.updateStats(result);
    updateStats(result);

    return result;
}

bool
MCTS::MCTSNode::operator<(const MCTS::MCTSNode &other) const
{
    return calcUCB() < other.calcUCB();
}

MCTS::MCTSNode &
MCTS::MCTSNode::expandChild(Board &b)
{
    static size_t nextId = 1;

    if (unexploredMoves_.empty())
        throw std::logic_error("Attempted to expand node with no unexplored moves");

    Move m = unexploredMoves_.back();

    unexploredMoves_.pop_back();

    children_.push_back(MCTS::MCTSNode(b, ourColour_, m, depth_ + 1, nextId++));

    return children_.back();
}

MCTS::MCTSNode &
MCTS::MCTSNode::selectChild()
{
    auto node = std::max_element(children_.begin(),
                                 children_.end());

    if (node == children_.end())
        throw std::logic_error("selectChild() called on horizon node");

    return *node;
}

Move
MCTS::MCTSNode::selectBestMove() const
{
    auto node = std::max_element(children_.begin(),
                                 children_.end(),
                                 [](const MCTSNode &a, const MCTSNode &b) -> bool
    {
        return a.calcValue() < b.calcValue();
    });

    if (node == children_.end())
        throw std::logic_error("selectBestChildNode() called on horizon node");

    return node->getMove();
}

float
MCTS::MCTSNode::calcValue() const
{
    return static_cast<float>(wins_) /
           static_cast<float>(visits_);
}

float
MCTS::MCTSNode::calcUCB() const
{
    const float C = std::sqrt(2);

    return calcValue() +
        C * std::sqrt(std::log(static_cast<float>(totalSimulations)) /
                 static_cast<float>(visits_));
}
