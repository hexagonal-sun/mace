#include "evaluate.h"

using psqt_t = std::array<std::array<int, 64>, 2>;

static const psqt_t pstPawn =
{
    // Middlegame
    {{
//A1                                H1
        0,   0,   0,   0,   0,   0,   0,   0,
        10,  20,  0,   5,   5,   0,   20,  10,
        -10, 0,   5,   15,  15,  5,   0,   -10,
        -15, -5,  10,  25,  25,  10,  -5,  -15,
        -15, -5,  5,   15,  15,  5,   -5,  -15,
        45,  55,  65,  75,  75,  65,  55,  45,
        105, 115, 125, 135, 135, 125, 115, 105,
        0,   0,   0,   0,   0,   0,   0,   0,
//A8                                H8
    },
    // Endgame
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        3,   1,   -1,  -3,  -3,  -1,  1,   3,
        3,   1,   -1,  -3,  -3,  -1,  1,   3,
        9,   6,   3,   0,   0,   3,   6,   9,
        12,  9,   6,   3,   3,   6,   9,   12,
        87,  85,  83,  81,  81,  83,  85,  87,
        159, 157, 155, 153, 153, 155, 157, 159,
        0,   0,   0,   0,   0,   0,   0,   0,
    }}
};

static const psqt_t pstKnight  =
{
    {{
        -50, -40, -30, -25, -25, -30, -40, -50,
        -35, -25, -15, -10, -10, -15, -25, -35,
        -20, -10,   0,   5,   5,   0, -10, -20,
        -10,   0,  10,  15,  15,  10,   0, -10,
        -5,   5,  15,  20,  20,  15,   5,  -5,
        -5,   5,  15,  20,  20,  15,   5,  -5,
        -20, -10,   0,   5,   5,   0, -10, -20,
        -135, -25, -15, -10, -10, -15, -25, -135
    },
    {
        -40, -30, -20, -15, -15, -20, -30, -40,
        -30, -20, -10,  -5,  -5, -10, -20, -30,
        -20, -10,   0,   5,   5,   0, -10, -20,
        -15,  -5,   5,  10,  10,   5,  -5, -15,
        -15,  -5,   5,  10,  10,   5,  -5, -15,
        -20, -10,   0,   5,   5,   0, -10, -20,
        -30, -20, -10,  -5,  -5, -10, -20, -30,
        -40, -30, -20, -15, -15, -20, -30, -40
    }}
};

static const psqt_t pstBishop =
{
    {{
        -2, -5, -16, -5, -5, -16, -5, -2,
        -5, 6,  3,   6,  6,  3,   6,  -5,
        -5, 5,  8,   10, 10, 8,   5,  -5,
        -5, 0,  10,  13, 13, 10,  0,  -5,
        -5, 0,  5,   13, 13, 5,   0,  -5,
        -5, 0,  5,   5,  5,  5,   0,  -5,
        -5, 0,  0,   0,  0,  0,   0,  -5,
        -5, -5, -5,  -5, -5, -5,  -5, -5
    },
    {
        -15, -10,  -8,  -5,  -5,  -8, -10, -15,
        -10,  -8,   0,   5,   5,   0,  -8, -10,
        -5,   0,   5,  10,  10,   5,   0,  -8,
        -5,   5,  10,  15,  15,  10,   5,  -5,
        -5,   5,  10,  15,  15,  10,   5,  -5,
        -8,   0,   5,  10,  10,   5,   0,  -8,
        -10,  -8,   0,   5,   5,   0,  -8, -10,
        -15, -10,  -8,  -5,  -5,  -8, -10, -15
    }}
};

static const psqt_t pstRook =
{
    {{
        0,  1,  2,  4,  4,  2,  1,  0,
        -4, -2, 0,  2,  2,  0,  -2, -4,
        -4, -2, 0,  2,  2,  0,  -2, -4,
        -4, -2, 0,  2,  2,  0,  -2, -4,
        -4, -2, 0,  2,  2,  0,  -2, -4,
        -4, -2, 0,  2,  2,  0,  -2, -4,
        15, 15, 15, 15, 15, 15, 15, 15,
        4,  4,  4,  4,  4,  4,  4,  4
    },
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        -2,   0,   0,   0,   0,   0,   0,  -2,
        -2,   0,   0,   0,   0,   0,   0,  -2,
        -2,   0,   0,   0,   0,   0,   0,  -2,
        -2,   0,   0,   0,   0,   0,   0,  -2,
        -2,   0,   0,   0,   0,   0,   0,  -2,
        15,  15,  15,  15,  15,  15,  15,  15,
        4,   4,   4,   4,   4,   4,   4,   4
    }}
};

static const psqt_t pstQueen =
{
    {{
            -5, -5, -5, -5, -5, -5, -5, -5,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0
        },
     {
         -24, -16, -12, -8, -8, -12, -16, -24,
         -16, -12, -4,  0,  0,  -4,  -12, -16,
         -12, -4,  0,   4,  4,  0,   -4,  -12,
         -8,  0,   4,   8,  8,  4,   0,   -8,
         -8,  0,   4,   8,  8,  4,   0,   -8,
         -12, -4,  0,   4,  4,  0,   -4,  -12,
         -16, -12, -4,  0,  0,  -4,  -12, -16,
         -24, -16, -12, -8, -8, -12, -16, -24
     }}
};

static const psqt_t pstKing =
{
    {{
        40,  50,  30,  10,  10,  30,  50,  40,
        30,  40,  20,  0,   0,   20,  40,  30,
        10,  20,  0,   -20, -20, 0,   20,  10,
        0,   10,  -10, -30, -30, -10, 10,  0,
        -10, 0,   -20, -40, -40, -20, 0,   -10,
        -20, -10, -30, -50, -50, -30, -10, -20,
        -30, -20, -40, -60, -60, -40, -20, -30,
        -40, -30, -50, -70, -70, -50, -30, -40
    },
    {
        -72, -48, -36, -24, -24, -36, -48, -72,
        -48, -24, -12, 0,   0,   -12, -24, -48,
        -36, -12, 0,   12,  12,  0,   -12, -36,
        -24, 0,   12,  24,  24,  12,  0,   -24,
        -24, 0,   12,  24,  24,  12,  0,   -24,
        -36, -12, 0,   12,  12,  0,   -12, -36,
        -48, -24, -12, 0,   0,   -12, -24, -48,
        -72, -48, -36, -24, -24, -36, -48, -72
    }}
};

static int colourCorrectMagnitude(int magnitude, Colour c)
{
    if (c == Colour::WHITE)
        return magnitude;

    return -magnitude;
}

static int getValueFromTable(Locus l, SquareState s,
                             const psqt_t &table,
                             int phase)
{
    int tableIndex = l.getArrayIndex();

    if (s.getColour() == Colour::BLACK)
        tableIndex = (tableIndex & 7) |
                     ((7 << 3) - (tableIndex & (7 << 3)));

    int openingEval = table[0][tableIndex];
    int endgameEval = table[1][tableIndex];
    int eval = ((openingEval * (256 - phase)) +
                (endgameEval * phase)) / 256;

    return colourCorrectMagnitude(eval, s.getColour());
}

static int getPSQTValue(Locus l, SquareState s, int phase)
{
    auto pieceType = s.getPieceType();

    switch (pieceType)
    {
    case PieceType::PAWN:
        return getValueFromTable(l, s, pstPawn, phase);
    case PieceType::KNIGHT:
        return getValueFromTable(l, s, pstKnight, phase);
    case PieceType::BISHOP:
        return getValueFromTable(l, s, pstBishop, phase);
    case PieceType::KING:
        return getValueFromTable(l, s, pstKing, phase);
    case PieceType::QUEEN:
        return getValueFromTable(l, s, pstQueen, phase);
    case PieceType::ROOK:
        return getValueFromTable(l, s, pstRook, phase);
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

int calcuatePhase(Board &b)
{
    static const std::map<PieceType, int> phaseWeights =
    {
        { PieceType::KING,   0 },
        { PieceType::PAWN,   0 },
        { PieceType::KNIGHT, 1 },
        { PieceType::BISHOP, 1 },
        { PieceType::ROOK,   2 },
        { PieceType::QUEEN,  4 }
    };

    const int totalPhase = phaseWeights.at(PieceType::PAWN) * 16 +
        phaseWeights.at(PieceType::KNIGHT) * 4 +
        phaseWeights.at(PieceType::BISHOP) * 4 +
        phaseWeights.at(PieceType::ROOK) * 4 +
        phaseWeights.at(PieceType::QUEEN) * 2;

    int phase = totalPhase;

    for (const auto pieceType : pieceTypes) {
        int count = 0;

        for (const auto colour : {Colour::WHITE, Colour::BLACK})
            count += b.pieceCounts.at(pieceType + colour);

        phase -= phaseWeights.at(pieceType) * count;
    }

    return (phase * 256 + (totalPhase / 2)) / totalPhase;
}

int getEvaluation(Board &b)
{
    int eval = 0;

    int phase = calcuatePhase(b);

    for (const auto [sq, loc] : b.getChessBoard())
    {
        if (!sq.isOccupied())
            continue;

        eval += getPieceValue(sq);
        eval += getPSQTValue(loc, sq, phase);
    }

    return eval;
}
