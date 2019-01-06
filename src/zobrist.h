#pragma once

#include <array>

#include "squareState.hpp"

class Board;

using ZobristHash = uint64_t;

class Zobrist
{
    std::array<std::array<uint64_t, 12>, 64> hashTable_;
    std::array<uint64_t, 2> colourHash_;
    std::array<uint64_t, 16> castlingHash_;
    std::array<uint64_t, 8> epHash_;
    size_t getZobPieceTypeIdx(SquareState sq) const;
public:
    Zobrist();
    ZobristHash getHash(const Board &b) const;
    void updateHash(ZobristHash &hash,
                    Locus loc,
                    SquareState before,
                    SquareState after) const;

    ZobristHash applyBoardState(ZobristHash boardHash,
                                const Board &b) const;
    struct Hash
    {
        std::size_t operator()(ZobristHash const& h) const noexcept
            {
                return h;
            }
    };
};

extern Zobrist zobHash;
