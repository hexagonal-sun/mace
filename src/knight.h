#pragma once

#include "piece.h"

class Knight : public Piece
{
public:
    Knight(Colour col);
    std::vector<Move> getCandidateMoves(const Board &b, Locus from) const;
    void printPiece(std::ostream &stream) const;
    unsigned int getValueMagnitude(void) const;
    PieceType getPieceType(void) const;
};
