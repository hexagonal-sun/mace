#include "board.h"
#include "knight.h"
#include "moveTypes/knight.hpp"

Knight::Knight(Colour col)
    : Piece(col)
{
}

moveList_t Knight::getCandidateMoves(const Board &b, Locus from) const
{
    return applyTranslationSpec(b, from, knightMoves, true);
}

void Knight::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('n');
}

unsigned int Knight::getValueMagnitude(void) const
{
    return 3;
}

PieceType Knight::getPieceType(void) const
{
    return PieceType::KNIGHT;
}
