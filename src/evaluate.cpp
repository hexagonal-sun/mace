#include "evaluate.h"

static const std::array<int, 64> pawnTable =
{
    0,  0,  0,   0,   0,   0,   0,  0,
    5,  10, 10,  -25, -25, 10,  10, 5,
    5,  -5, -10, 0,   0,   -10, -5, 5,
    0,  0,  0,   25,  25,  0,   0,  0,
    5,  5,  10,  27,  27,  10,  5,  5,
    10, 10, 20,  30,  30,  20,  10, 10,
    50, 50, 50,  50,  50,  50,  50, 50,
    0,  0,  0,   0,   0,   0,   0,  0,
};

static const std::array<int, 64> knightTable =
{
    -50, -40, -20, -30, -30, -20, -40, -50,
    -40, -20, 0,   5,   5,   0,   -20, -40,
    -30, 5,   10,  15,  15,  10,  5,   -30,
    -30, 0,   15,  20,  20,  15,  0,   -30,
    -30, 5,   15,  20,  20,  15,  5,   -30,
    -30, 0,   10,  15,  15,  10,  0,   -30,
    -40, -20, 0,   0,   0,   0,   -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50,
};

static const std::array<int, 64> bishopTable =
{
    -20, -10, -40, -10, -10, -40, -10, -20,
    -10, 5,   0,   0,   0,   0,   5,   -10,
    -10, 10,  10,  10,  10,  10,  10,  -10,
    -10, 0,   10,  10,  10,  10,  0,   -10,
    -10, 5,   5,   10,  10,  5,   5,   -10,
    -10, 0,   5,   10,  10,  5,   0,   -10,
    -10, 0,   0,   0,   0,   0,   0,   -10,
    -20, -10, -10, -10, -10, -10, -10, -20,
};

static const std::array<int, 64> kingTable =
{
    20,  30,  10,  0,   0,   10,  30,  20,
    20,  20,  0,   0,   0,   0,   20,  20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
};

static int colourCorrectMagnitude(int magnitude, Colour c)
{
    if (c == Colour::WHITE)
        return magnitude;

    return -magnitude;
}

static int getValueFromTable(Locus l, SquareState s,
                             const std::array<int, 64> &table)
{
    int idx = l.getIndex();

    // Convert from 0x88 board coords to PSQT coords.
    int tableIndex = (idx & 0xf) | ((idx & 0xf0) >> 1);

    if (s.getColour() == Colour::BLACK)
        tableIndex = (tableIndex & 7) |
                     ((7 << 3) - (tableIndex & (7 << 3)));

    return colourCorrectMagnitude(table[tableIndex], s.getColour());
}

static int getPSQTValue(Locus l, SquareState s)
{
    auto pieceType = s.getPieceType();

    switch (pieceType)
    {
    case PieceType::PAWN:
        return getValueFromTable(l, s, pawnTable);
    case PieceType::KNIGHT:
        return getValueFromTable(l, s, knightTable);
    case PieceType::BISHOP:
        return getValueFromTable(l, s, bishopTable);
    case PieceType::KING:
        return getValueFromTable(l, s, kingTable);
    default:
        return 0;
    }
}


static size_t getPieceMag(SquareState s)
{
    switch (s.getPieceType())
    {
    case PieceType::PAWN:
        return 100;
    case PieceType::BISHOP:
    case PieceType::KNIGHT:
        return 300;
    case PieceType::ROOK:
        return 500;
    case PieceType::QUEEN:
        return 900;
    default:
        return 0;
    }
}

static int getPieceValue(SquareState s)
{
    return colourCorrectMagnitude(getPieceMag(s),
                                  s.getColour());
}

int getEvaluation(Board &b)
{
    int eval = 0;

    for (const auto [sq, loc] : b.getChessBoard())
    {
        if (!sq.isOccupied())
            continue;

        eval += getPieceValue(sq);
        eval += getPSQTValue(loc, sq);
    }

    return eval;
}
