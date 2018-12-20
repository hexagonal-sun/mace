#include "board.h"
#include "bishop.h"

Bishop::Bishop(Colour col)
    : Piece(col)
{
}

moveList_t Bishop::getCandidateMoves(const Board &b, Locus from) const
{
    return applyTranslationSpec(b, from, diagonalMoves, false);
}

void Bishop::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('b');
}

unsigned int Bishop::getValueMagnitude(void) const
{
    return 3;
}

PieceType Bishop::getPieceType(void) const
{
    return PieceType::BISHOP;
}
