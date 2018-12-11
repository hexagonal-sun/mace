#pragma once
#include "board.h"

class Mover
{
public:
    Mover(const Move &m, Board &b)
        : move_(m), board_(b), takenPiece(nullptr)
        {
            auto &sourceSquare = board_[std::get<0>(move_)];
            auto &destSquare = board_[std::get<1>(move_)];

            if (destSquare.isOccupied())
                takenPiece = destSquare.getPiece();

            destSquare.setPiece(sourceSquare.getPiece());
            sourceSquare.setEmpty();

            board_.getNextMoveColour() = getOppositeColour(board_.getNextMoveColour());
        }
    ~Mover()
        {
            auto &sourceSquare = board_[std::get<1>(move_)];
            auto &destSquare = board_[std::get<0>(move_)];

            destSquare.setPiece(sourceSquare.getPiece());

            if (takenPiece)
                sourceSquare.setPiece(takenPiece);
            else
                sourceSquare.setEmpty();

            board_.getNextMoveColour() = getOppositeColour(board_.getNextMoveColour());
        }
    Mover(const Mover &m) = delete;
    Mover(Mover &&m) = delete;
private:
    const Move &move_;
    Board &board_;
    std::shared_ptr<Piece> takenPiece;
};
