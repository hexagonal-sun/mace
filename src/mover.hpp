#pragma once
#include "board.h"

enum class MoverType
{
    COMMIT,
    REVERT
};

template<MoverType t>
class Mover
{
public:
    Mover(const Move &m, Board &b)
        : move_(m), board_(b),
          enPassantCapture_(b.getEnPassantLocus()),
          castlingRights_(b.getCastlingRights()),
          halfMoveClock_(b.getHalfMoveClock())
        {
            auto sourceSquare = board_[move_.getFrom()];
            auto destSquare = board_[move_.getTo()];
            auto movingPieceType = sourceSquare.getPieceType();
            auto movingPieceColour = sourceSquare.getColour();

            b.getEnPassantLocus() = Locus();

            takenSquare = destSquare;
            destSquare = sourceSquare;
            sourceSquare = SquareState::UNOCCUPIED;

            if (movingPieceType == PieceType::KING) {
                board_.getKingLocus(movingPieceColour) = move_.getTo();

                // Eliminate all castling rights for this particular
                // colour on king moves.
                const auto mask = getCastlingMask(movingPieceColour);

                board_.getCastlingRights() &= ~mask;
            }

            // Eliminate castling rights for rook moves.
            if (movingPieceType == PieceType::ROOK &&
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

            if (takenSquare.isOccupied() &&
                takenSquare.getPieceType() == PieceType::ROOK &&
                (move_.getTo().getFile() == File::A ||
                 move_.getTo().getFile() == File::H) &&
                (move_.getTo().getRank() ==
                 (takenSquare.getColour() == Colour::WHITE ?
                  Rank::ONE : Rank::EIGHT))) {
                auto mask = getCastlingMask(takenSquare.getColour());

                mask &= move_.getTo().getFile() == File::A ?
                    getQueenSideMask() : getKingSideMask();

                board_.getCastlingRights() &= ~mask;
            }

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

                board_[castlingRookDest_] = board_[castlingRookSource_];
                board_[castlingRookSource_] = SquareState::UNOCCUPIED;

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

                takenSquare = board_[takenPieceLocus];
                board_[takenPieceLocus] = SquareState::UNOCCUPIED;
                enPassantTake_ = takenPieceLocus;
            }

            SquareState promoSquare;

            switch(move_.getType())
            {
            case MoveType::PROMOTE_BISHOP:
                promoSquare = PieceType::BISHOP + movingPieceColour;
                break;
            case MoveType::PROMOTE_ROOK:
                promoSquare = PieceType::ROOK + movingPieceColour;
                break;
            case MoveType::PROMOTE_KNIGHT:
                promoSquare = PieceType::KNIGHT + movingPieceColour;
                break;
            case MoveType::PROMOTE_QUEEN:
                promoSquare = PieceType::QUEEN + movingPieceColour;
                break;
            default:
                break;
            }

            if (promoSquare.isOccupied()) {
                destSquare = promoSquare;
                board_.pieceCounts[PieceType::PAWN + movingPieceColour]--;
                board_.pieceCounts[promoSquare]++;
            }

            if (takenSquare.isOccupied())
                board_.pieceCounts[takenSquare]--;

            if (movingPieceType == PieceType::PAWN ||
                move_.getType() == MoveType::TAKE)
                board_.getHalfMoveClock() = 0;
            else
                board_.getHalfMoveClock()++;

            board_.getNextMoveColour() = getOppositeColour(board_.getNextMoveColour());

            if (t == MoverType::COMMIT)
                board_.pushPosition();
        }
    ~Mover()
        {
            if (t == MoverType::COMMIT)
                return;

            auto sourceSquare = board_[move_.getTo()];
            auto destSquare = board_[move_.getFrom()];

            destSquare = sourceSquare;
            sourceSquare = SquareState::UNOCCUPIED;

            if (isPromotion(move_.getType())) {
                board_.pieceCounts[destSquare]--;
                destSquare = PieceType::PAWN + destSquare.getColour();
                board_.pieceCounts[destSquare]++;
            }

            if (move_.getType() == MoveType::ENPASSANT_TAKE)
                board_[enPassantTake_] = takenSquare;
            else
                sourceSquare = takenSquare;

            if (takenSquare.isOccupied())
                board_.pieceCounts[takenSquare]++;

            if (destSquare.getPieceType() == PieceType::KING)
                board_.getKingLocus(destSquare.getColour()) = move_.getFrom();

            // Restore the rooks position on castling moves.
            if (isCastling(move_.getType())) {
                board_[castlingRookSource_] = board_[castlingRookDest_];
                board_[castlingRookDest_] = SquareState::UNOCCUPIED;
            }


            board_.getNextMoveColour() = getOppositeColour(board_.getNextMoveColour());

            board_.getEnPassantLocus() = enPassantCapture_;
            board_.getCastlingRights() = castlingRights_;
            board_.getHalfMoveClock() = halfMoveClock_;
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
    SquareState takenSquare;
    Locus enPassantCapture_;
    CastlingRights castlingRights_;
    Locus enPassantTake_;
    Locus castlingRookSource_;
    Locus castlingRookDest_;
    size_t halfMoveClock_;
};
