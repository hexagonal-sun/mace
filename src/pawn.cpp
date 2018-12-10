#include "pawn.h"

Pawn::Pawn(Colour col)
    : Piece(col)
{
}

std::vector<Board> Pawn::getCandidateMoves(const Board &b, Locus from) const
{
    const auto dir = getColour() == Colour::WHITE ? Direction::NORTH : Direction::SOUTH;
    const auto startingRank = getColour() == Colour::WHITE ? Rank::TWO : Rank::SEVEN;
    std::vector<Board> ret;

    auto newLoc = from.translate(dir);

    if (canMoveToSquare(b, newLoc, getColour())) {
        ret.push_back(b.move(from, newLoc));

        // We can only move a pawn forward two squares if it isn't
        // obstructed at `newLoc' and it's on it's starting rank.
        if (from.getRank() == startingRank) {
            auto doubleMove = newLoc.translate(dir);
            if (canMoveToSquare(b, doubleMove, getColour()))
                ret.push_back(b.move(from, doubleMove));
        }
    }

    // See if we can take any pieces.
    for (const auto &takeDir : {Direction::EAST, Direction::WEST})
        try
        {
            const auto &takeLoc = from.translate(dir).translate(takeDir);
            const auto &square = b[takeLoc];
            if (square.isOccupied() &&
                square.getPiece()->getColour() == getOppositeColour(getColour()))
                ret.push_back(b.move(from, takeLoc));
        }
        catch (std::domain_error &e)
        {
            continue;
        }

    return ret;
}

void Pawn::printPiece(std::ostream &stream) const
{
    stream << formatPieceChar('p');
}

unsigned int Pawn::getValueMagnitude(void) const
{
    return 1;
}

PieceType Pawn::getPieceType(void) const
{
    return PieceType::PAWN;
}
