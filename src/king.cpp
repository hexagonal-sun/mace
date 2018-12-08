#include "king.h"

King::King(Colour col)
    : Piece(col)
{
}

std::vector<Board> King::getCandidateMoves(Board &b, Locus from) const
{
    return std::vector<Board>();
}

void King::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('k');
}

unsigned int King::getValueMagnitude(void) const
{
    return 1024;
}
