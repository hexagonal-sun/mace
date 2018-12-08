#pragma once

#include "piece.h"

class Queen : public Piece
{
public:
    Queen(Colour col);
    std::vector<Board> getCandidateMoves(Board &b, Locus l) const;
    void printPiece(std::ostream &stream) const;
    unsigned int getValueMagnitude(void) const;
};
