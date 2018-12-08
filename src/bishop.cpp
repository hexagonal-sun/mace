#include "bishop.h"

Bishop::Bishop(Colour col)
    : Piece(col)
{
}

std::vector<Board> Bishop::getCandidateMoves(Board &b, Locus from) const
{
    return std::vector<Board>();
}

void Bishop::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('b');
}

unsigned int Bishop::getValueMagnitude(void) const
{
    return 3;
}
