#pragma once

#include "piece.h"

class King : public Piece
{
public:
    King(Colour col);
    std::vector<Board> getCandidateMoves(Board &b, Locus from) const;
    void printPiece(std::ostream &stream) const;
    unsigned int getValueMagnitude(void) const;
};
