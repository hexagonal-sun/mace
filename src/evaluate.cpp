#include "evaluate.h"

static size_t getPieceMag(SquareState s)
{
    switch (s.getPieceType())
    {
    case PieceType::PAWN:
        return 1;
    case PieceType::BISHOP:
    case PieceType::KNIGHT:
        return 3;
    case PieceType::ROOK:
        return 5;
    case PieceType::QUEEN:
        return 9;
    default:
        return 0;
    }
}

int getEvaluation(Board &b)
{
    int eval = 0;

    for (const auto [sq, loc] : b.getChessBoard())
    {
        if (!sq.isOccupied())
            continue;

        auto valueMagnitude = getPieceMag(sq);

        eval += sq.getColour() == Colour::WHITE ?
            valueMagnitude : -valueMagnitude;
    }

    return eval;
}
