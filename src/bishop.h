#pragma once

#include "piece.h"

class Bishop : public Piece
{
public:
    Bishop(Colour col);
    std::vector<Board> getCandidateMoves(const Board &b, Locus from) const;
    void printPiece(std::ostream &stream) const;
    unsigned int getValueMagnitude(void) const;
    PieceType getPieceType(void) const;
};
