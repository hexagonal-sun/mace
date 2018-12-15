#pragma once
#include "board.h"

class Mover
{
public:
    Mover(const Move &m, Board &b)
        : move_(m), board_(b), takenPiece(nullptr),
          enPassantCapture_(b.getEnPassantLocus())
        {
            auto &sourceSquare = board_[move_.getFrom()];
            auto &destSquare = board_[move_.getTo()];
            auto movingPiece = sourceSquare.getPiece();
            auto movingPieceColour = movingPiece->getColour();

            b.getEnPassantLocus() = Locus();

            if (destSquare.isOccupied())
                takenPiece = destSquare.getPiece();

            destSquare.setPiece(sourceSquare.getPiece());
            sourceSquare.setEmpty();

            if (movingPiece->getPieceType() == PieceType::KING)
                board_.getKingLocus(movingPiece->getColour()) = move_.getTo();

            if (move_.getType() == MoveType::ENPASSANT_ADVANCE)
                board_.getEnPassantLocus() = enPassantTransform(m.getTo(),
                                                                movingPieceColour);

            if (move_.getType() == MoveType::ENPASSANT_TAKE) {
                auto takenPieceLocus = enPassantTransform(m.getTo(),
                                                          movingPieceColour);

                takenPiece = board_[takenPieceLocus].getPiece();
                board_[takenPieceLocus].setEmpty();
                enPassantTake_ = takenPieceLocus;
            }

            board_.getNextMoveColour() = getOppositeColour(board_.getNextMoveColour());
        }
    ~Mover()
        {
            auto &sourceSquare = board_[move_.getTo()];
            auto &destSquare = board_[move_.getFrom()];
            auto movingPiece = sourceSquare.getPiece();

            destSquare.setPiece(sourceSquare.getPiece());
            sourceSquare.setEmpty();

            if (takenPiece)
                if (move_.getType() == MoveType::ENPASSANT_TAKE)
                    board_[enPassantTake_].setPiece(takenPiece);
                else
                    sourceSquare.setPiece(takenPiece);

            if (movingPiece->getPieceType() == PieceType::KING)
                board_.getKingLocus(movingPiece->getColour()) = move_.getFrom();

            board_.getNextMoveColour() = getOppositeColour(board_.getNextMoveColour());

            board_.getEnPassantLocus() = enPassantCapture_;
        }
    Mover(const Mover &m) = delete;
    Mover(Mover &&m) = delete;
private:
    const Locus enPassantTransform(Locus l, Colour c) const
        {
            auto dir = c == Colour::WHITE ? -0x10 : 0x10;
            auto file = l.getFile();
            auto rank = l.getRank();
            auto enPassantRank = static_cast<Rank>(static_cast<int>(rank) + dir);
            return Locus(enPassantRank, file);
        }
    const Move &move_;
    Locus enPassantCapture_;
    Board &board_;
    Locus enPassantTake_;
    Piece *takenPiece;
};
