#include "board.h"
#include "queen.h"

Queen::Queen(Colour col)
    : Piece(col)
{
}

moveList_t Queen::getCandidateMoves(const Board &b, Locus from) const
{
    return applyTranslationSpec(b, from, orthoDiagonalMoves, false);
}

void Queen::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('q');
}

unsigned int Queen::getValueMagnitude(void) const
{
    return 9;
}

PieceType Queen::getPieceType(void) const
{
    return PieceType::QUEEN;
}
