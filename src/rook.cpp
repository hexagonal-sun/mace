#include "rook.h"

Rook::Rook(Colour col)
    : Piece(col)
{
}

std::vector<Board> Rook::getCandidateMoves(Board &b, Locus from) const
{
    std::vector<Board> ret;

    

    return ret;
}

void Rook::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('r');
}

unsigned int Rook::getValueMagnitude(void) const
{
    return 5;
}
