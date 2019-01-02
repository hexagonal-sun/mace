#include <random>
#include "zobrist.h"

Zobrist::Zobrist()
{
    static std::random_device rd;
    static std::mt19937_64 gen(rd());

    // Populate the hash table with random values.
    for (auto &x : hashTable_)
        for (auto &y : x)
            y = gen();
}

uint64_t Zobrist::getHash(const Board &b) const
{
    uint64_t hash = 0;
    size_t sqIdx = 0;

    for (const auto [sq, loc] : b.getChessBoard())
    {
        if (!sq.isOccupied())
            continue;

        auto pieceIdx = (static_cast<int>(sq.getPieceType()) >> 1) - 1;

        if (b.getNextMoveColour() == Colour::BLACK)
            pieceIdx += 6;

        hash ^= hashTable_[sqIdx][pieceIdx];

        sqIdx++;
    }

    return hash;
}

Zobrist zobHash;
