#pragma once

#include <array>

#include "board.h"

class Zobrist
{
    std::array<std::array<uint64_t, 12>, 64> hashTable_;
public:
    Zobrist();
    uint64_t getHash(const Board &b) const;
};

extern Zobrist zobHash;
