#include "zobrist.h"
#include "move.hpp"

enum class NodeType
{
    EXACT,
    LOWERBOUND,
    UPPERBOUND
};

struct TTData
{
    ZobristHash hash;
    int value;
    size_t depth;
    Move bestMove;
    NodeType nt;
};

class TranspositionTable
{
    std::vector<TTData> data_;
    uint64_t mask;
public:
    TranspositionTable(size_t bits)
        : data_(1 << bits)
    {
        mask = (1 << bits) - 1;
    }

    TTData &operator[](const ZobristHash hash)
        {
            return data_[hash & mask];
        }
    const TTData &operator[](const ZobristHash hash) const
        {
            return data_[hash & mask];
        }
    const TTData at(ZobristHash hash) const
        {
            return data_.at(hash & mask);
        }
};

extern TranspositionTable TTable;
