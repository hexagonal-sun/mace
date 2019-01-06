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
    size_t size_;
    TTData *data_;
public:
    TranspositionTable(size_t sz)
        : size_(sz), data_(new TTData[sz]())
        {
        }

    ~TranspositionTable()
        {
            delete data_;
        }

    TTData &operator[](const ZobristHash hash)
        {
            return data_[hash % size_];
        }
};

extern TranspositionTable TTable;
