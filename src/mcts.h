#include <boost/container/small_vector.hpp>
#include <ostream>

#include "board.h"

class MCTS
{
public:
    MCTS(Board &b)
        : b_(b), root_(b_)
        {
        }

    Move selectMove(size_t numIterations, Board &b);
    void dumpTree(std::string fileName) const;

private:
    class MCTSNode;

    class MCTSNode
    {
    public:
        MCTSNode(Board &b);
        bool MCTS(Board &b);
        bool operator<(const MCTSNode &other) const;
        Move getMove() const;
        void dumpNode(std::ostream &os) const;
        size_t getId() const;
        Move selectBestMove() const;
    private:
        MCTSNode(Board &b, Colour ourColour, Move m, size_t depth,
                 size_t id);
        MCTSNode &expandChild(Board &b);
        MCTSNode &selectChild();
        bool doMCTS(Board &b);
        bool rollout(Move move, Board &b) const;
        void updateStats(bool didWin);
        void addChild();
        float calcValue() const;
        float calcUCB() const;
        Colour ourColour_;
        std::vector<MCTSNode> children_;
        moveList_t unexploredMoves_;
        Move move_;
        size_t visits_;
        size_t wins_;
        size_t depth_;
        size_t id_;
    };

    Board &b_;
    MCTSNode root_;
};
