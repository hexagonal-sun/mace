#include <random>
#include "board.h"
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

size_t Zobrist::getZobPieceTypeIdx(SquareState sq) const
{
    size_t pieceIdx = getPieceTypeIdx(sq.getPieceType());

    if (sq.getColour() == Colour::BLACK)
        pieceIdx += 6;

    return pieceIdx;
}

ZobristHash Zobrist::getHash(const Board &b) const
{
    ZobristHash hash = 0;

    for (const auto [sq, loc] : b.getChessBoard())
    {
        if (!sq.isOccupied())
            continue;

        hash ^= hashTable_[loc.getArrayIndex()][getZobPieceTypeIdx(sq)];
    }

    return hash;
}

void Zobrist::updateHash(ZobristHash &hash,
                         Locus loc,
                         SquareState before,
                         SquareState after) const
{
    if (before.isOccupied())
        hash ^= hashTable_[loc.getArrayIndex()][getZobPieceTypeIdx(before)];

    if (after.isOccupied())
        hash ^= hashTable_[loc.getArrayIndex()][getZobPieceTypeIdx(after)];
}

Zobrist zobHash;
