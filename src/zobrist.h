#pragma once

#include <array>

#include "locus.h"
#include "squareState.hpp"

class Board;

using ZobristHash = uint64_t;

class Zobrist
{
    std::array<std::array<uint64_t, 12>, 64> hashTable_;
    size_t getZobPieceTypeIdx(SquareState sq) const;
public:
    Zobrist();
    ZobristHash getHash(const Board &b) const;
    void updateHash(ZobristHash &hash,
                    Locus loc,
                    SquareState before,
                    SquareState after) const;
    struct Hash
    {
        std::size_t operator()(ZobristHash const& h) const noexcept
            {
                return h;
            }
    };
};

extern Zobrist zobHash;
