#pragma once
#include "board.h"

class Mover
{
public:
    Mover(const Move &m, Board &b)
        : move_(m), board_(b), takenPiece(nullptr)
        {
            auto &sourceSquare = board_[move_.getFrom()];
            auto &destSquare = board_[move_.getTo()];
            auto movingPiece = sourceSquare.getPiece();

            if (destSquare.isOccupied())
                takenPiece = destSquare.getPiece();

            destSquare.setPiece(sourceSquare.getPiece());
            sourceSquare.setEmpty();

            if (movingPiece->getPieceType() == PieceType::KING)
                board_.getKingLocus(movingPiece->getColour()) = move_.getTo();

            board_.getNextMoveColour() = getOppositeColour(board_.getNextMoveColour());
        }
    ~Mover()
        {
            auto &sourceSquare = board_[move_.getTo()];
            auto &destSquare = board_[move_.getFrom()];
            auto movingPiece = sourceSquare.getPiece();

            destSquare.setPiece(sourceSquare.getPiece());

            if (takenPiece)
                sourceSquare.setPiece(takenPiece);
            else
                sourceSquare.setEmpty();

            if (movingPiece->getPieceType() == PieceType::KING)
                board_.getKingLocus(movingPiece->getColour()) = move_.getFrom();

            board_.getNextMoveColour() = getOppositeColour(board_.getNextMoveColour());
        }
    Mover(const Mover &m) = delete;
    Mover(Mover &&m) = delete;
private:
    const Move &move_;
    Board &board_;
    Piece *takenPiece;
};
