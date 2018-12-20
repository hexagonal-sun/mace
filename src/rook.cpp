#include "board.h"
#include "rook.h"
#include "movementTypes.hpp"

Rook::Rook(Colour col)
    : Piece(col)
{
}

moveList_t Rook::getCandidateMoves(const Board &b, Locus from) const
{
    return applyTranslationSpec(b, from, orthogonalMoves, false);
}

void Rook::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('r');
}

unsigned int Rook::getValueMagnitude(void) const
{
    return 5;
}

PieceType Rook::getPieceType(void) const
{
    return PieceType::ROOK;
}
