#include "pawn.h"

Pawn::Pawn(Colour col)
    : Piece(col)
{
}

moveList_t Pawn::getCandidateMoves(const Board &b, Locus from) const
{
    const auto dir = getColour() == Colour::WHITE ? Direction::NORTH : Direction::SOUTH;
    const auto startingRank = getColour() == Colour::WHITE ? Rank::TWO : Rank::SEVEN;
    moveList_t ret;

    auto newLoc = from.translate(dir);

    if (b[newLoc].getSquareType(getColour()) == SquareType::EMPTY) {
        ret.push_back(Move(from, newLoc, MoveType::UNOCCUPIED));

        // We can only move a pawn forward two squares if it isn't
        // obstructed at `newLoc' and it's on it's starting rank.
        if (from.getRank() == startingRank) {
            auto doubleMove = newLoc.translate(dir);
            if (b[doubleMove].getSquareType(getColour()) == SquareType::EMPTY)
                ret.push_back(Move(from, doubleMove,
                                   MoveType::UNOCCUPIED));
        }
    }

    // See if we can take any pieces.
    for (const auto &takeDir : {Direction::EAST, Direction::WEST}) {
        const auto &takeLoc = from.translate(dir).translate(takeDir);

        if (!takeLoc.isValid())
            continue;

        if (b[takeLoc].getSquareType(getColour()) == SquareType::TAKE)
            ret.push_back(Move(from, takeLoc, MoveType::TAKE));
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
