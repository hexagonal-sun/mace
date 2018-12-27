#pragma once

#include "move.hpp"

class MoveStack
{
    moveList_t &stack_;
public:
    MoveStack(moveList_t &stack, Move m)
        : stack_(stack)
        {
            stack_.push_back(m);
        }
    ~MoveStack()
        {
            stack_.pop_back();
        }
};
