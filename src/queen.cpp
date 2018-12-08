#include "queen.h"

Queen::Queen(Colour col)
    : Piece(col)
{
}

std::vector<Board> Queen::getCandidateMoves(Board &b, Locus l) const
{
    return std::vector<Board>();
}

void Queen::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('q');
}

unsigned int Queen::getValueMagnitude(void) const
{
    return 9;
}
