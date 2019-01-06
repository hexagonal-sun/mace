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

    for (auto &cHash : colourHash_)
        cHash = gen();

    for (auto &cHash : castlingHash_)
        cHash = gen();

    for (auto &eHash : epHash_)
        eHash = gen();
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

    return applyBoardState(hash, b);
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

ZobristHash Zobrist::applyBoardState(ZobristHash hash,
                                     const Board &b) const
{
    size_t colourIdx = static_cast<size_t>(b.getNextMoveColour());
    Locus epl = b.getEnPassantLocus();

    hash ^= colourHash_[colourIdx];
    hash ^= castlingHash_[b.getCastlingRights().to_ullong()];

    if (epl.isValid())
        hash ^= epHash_[static_cast<size_t>(epl.getFile())];

    return hash;
}

Zobrist zobHash;
