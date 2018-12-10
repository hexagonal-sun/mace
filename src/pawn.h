#pragma once

#include "piece.h"

class Pawn : public Piece
{
public:
    Pawn(Colour col);
    std::vector<Move> getCandidateMoves(const Board &b, Locus l) const;
    void printPiece(std::ostream &stream) const;
    unsigned int getValueMagnitude(void) const;
    PieceType getPieceType(void) const;
};
