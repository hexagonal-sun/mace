#pragma once
#include <vector>

enum class PieceType
{
    UNOCCUPIED = 0,
    PAWN       = (1 << 1),
    KING       = (2 << 1),
    QUEEN      = (3 << 1),
    BISHOP     = (4 << 1),
    KNIGHT     = (5 << 1),
    ROOK       = (6 << 1)
};

static const std::vector<PieceType> pieceTypes =
{
    PieceType::PAWN,
    PieceType::KING,
    PieceType::QUEEN,
    PieceType::ROOK,
    PieceType::KNIGHT,
    PieceType::BISHOP
};
