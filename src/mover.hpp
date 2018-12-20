#pragma once
#include "board.h"
#include "bishop.h"
#include "rook.h"
#include "knight.h"
#include "queen.h"

class Mover
{
public:
    Mover(const Move &m, Board &b)
        : move_(m), board_(b), takenPiece(nullptr),
          enPassantCapture_(b.getEnPassantLocus()),
          castlingRights_(b.getCastlingRights()),
          promotedPawn_(nullptr)
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

            if (movingPiece->getPieceType() == PieceType::KING) {
                board_.getKingLocus(movingPiece->getColour()) = move_.getTo();

                // Eliminate all castling rights for this particular
                // colour on king moves.
                const auto mask = getCastlingMask(movingPieceColour);

                board_.getCastlingRights() &= ~mask;
            }

            // Eliminate castling rights for rook moves.
            if (movingPiece->getPieceType() == PieceType::ROOK &&
                (move_.getFrom().getFile() == File::A ||
                 move_.getFrom().getFile() == File::H))
            {
                auto mask = getCastlingMask(movingPieceColour);

                if (move_.getFrom().getFile() == File::A)
                    mask &= getQueenSideMask();
                else
                    mask &= getKingSideMask();

                board_.getCastlingRights() &= ~mask;
            }

            // TODO: If a rook is captured, update the appropriate
            // castling rights.

            if (move_.getType() == MoveType::CASTLE_KINGSIDE ||
                move_.getType() == MoveType::CASTLE_QUEENSIDE) {
                auto rank = movingPieceColour == Colour::WHITE ?
                    Rank::ONE : Rank::EIGHT;

                if (move_.getType() == MoveType::CASTLE_KINGSIDE) {
                    castlingRookSource_ = rank + File::H;
                    castlingRookDest_ = rank + File::F;
                } else {
                    castlingRookSource_ = rank + File::A;
                    castlingRookDest_ = rank + File::D;
                }

                board_[castlingRookDest_].setPiece(board_[castlingRookSource_].getPiece());
                board_[castlingRookSource_].setEmpty();

                // Clear all castling rights for this colour after
                // castling has occurred.
                board_.getCastlingRights() &= ~getCastlingMask(movingPieceColour);
            }

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

            Piece *promotionPiece = nullptr;

            switch(move_.getType())
            {
            case MoveType::PROMOTE_BISHOP:
                promotionPiece = new Bishop(movingPieceColour);
                break;
            case MoveType::PROMOTE_ROOK:
                promotionPiece = new Rook(movingPieceColour);
                break;
            case MoveType::PROMOTE_KNIGHT:
                promotionPiece = new Knight(movingPieceColour);
                break;
            case MoveType::PROMOTE_QUEEN:
                promotionPiece = new Queen(movingPieceColour);
                break;
            default:
                break;
            }

            if (promotionPiece) {
                promotedPawn_ = destSquare.getPiece();
                destSquare.setPiece(promotionPiece);
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

            if (promotedPawn_) {
                Piece *p = destSquare.getPiece();
                destSquare.setEmpty();
                delete p;
                destSquare.setPiece(promotedPawn_);
                movingPiece = promotedPawn_;
            }

            if (takenPiece){
                if (move_.getType() == MoveType::ENPASSANT_TAKE)
                    board_[enPassantTake_].setPiece(takenPiece);
                else
                    sourceSquare.setPiece(takenPiece);
            }

            if (movingPiece->getPieceType() == PieceType::KING)
                board_.getKingLocus(movingPiece->getColour()) = move_.getFrom();

            // Restore the rooks position on castling moves.
            if (move_.getType() == MoveType::CASTLE_KINGSIDE ||
                move_.getType() == MoveType::CASTLE_QUEENSIDE) {
                board_[castlingRookSource_].setPiece(board_[castlingRookDest_].getPiece());
                board_[castlingRookDest_].setEmpty();
            }


            board_.getNextMoveColour() = getOppositeColour(board_.getNextMoveColour());

            board_.getEnPassantLocus() = enPassantCapture_;
            board_.getCastlingRights() = castlingRights_;
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
    Board &board_;
    Piece *takenPiece;
    Locus enPassantCapture_;
    CastlingRights castlingRights_;
    Piece *promotedPawn_;
    Locus enPassantTake_;
    Locus castlingRookSource_;
    Locus castlingRookDest_;
};
