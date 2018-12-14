#pragma once

#include "piece.h"

class Queen : public Piece
{
public:
    Queen(Colour col);
    moveList_t getCandidateMoves(const Board &b, Locus from) const;
    void printPiece(std::ostream &stream) const;
    unsigned int getValueMagnitude(void) const;
    PieceType getPieceType(void) const;
};
