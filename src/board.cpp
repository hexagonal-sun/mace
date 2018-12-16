#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <locale>
#include <iostream>
#include <regex>

#include "board.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "queen.h"
#include "king.h"
#include "bishop.h"
#include "boardSquare.h"
#include "mover.hpp"

Board::Board(Colour nextMoveColour)
    : nextMoveColour_(nextMoveColour)
{
}

const BoardSquare & Board::getSquare(std::string name) const
{
    if (name.length() != 2)
        throw std::invalid_argument("square specifier of incorrect format");

    char rank = name[1];
    char file = name[0];

    return board_.at(Locus(rank, file));
}

Move Board::validateMove(std::string from, std::string to)
{
    return validateMove(getSquare(from).getLocus(), getSquare(to).getLocus());
}

const Colour Board::getNextMoveColour(void) const
{
    return nextMoveColour_;
}

Colour & Board::getNextMoveColour(void)
{
    return nextMoveColour_;
}

Move Board::validateMove(const Locus & from, const Locus &to)
{
    const auto &allowedMoves = getAllCandidateMoves();

    for (const auto &m : allowedMoves) {
        if (m.getFrom() == from && m.getTo() == to)
            return m;
    }

    return Move();
}

void Board::printBoard(std::ostream &stream) const
{
    for (const auto rank : RANKS)
    {
        for (const auto file : FILES) {
            board_.at(Locus(rank, file)).printSquare(stream);

            stream << " ";
        }

        stream << "\n";
    }

    stream << "To move: " << colourNames.at(nextMoveColour_) << "\n";
}

locusList_t Board::locatePiece(Colour c, PieceType t) const
{
    locusList_t ret;

    for (const auto &square : board_) {
        if (square.isOccupied() &&
            square.getPiece()->getColour() == c &&
            square.getPiece()->getPieceType() == t)
            ret.push_back(square.getLocus());
    }

    return ret;
}

bool Board::isPieceUnderAttack(Locus l) const
{
    const auto &pieceColour = board_.at(l).getPiece()->getColour();

    bool ret = false;

    forEachPieceMoves(getOppositeColour(pieceColour),
                      [&](Piece *piece,
                          const moveList_t &moves)
    {
        for (const auto &move : moves)
            if (move.getTo() == l) {
                ret = true;
                return false;
            }
        return true;
    });

    return ret;
}

const bool Board::isInCheck(Colour kingsColour) const
{
    const auto &kingLocus = getKingLocus(kingsColour);

    return isPieceUnderAttack(kingLocus);
}


const int Board::getEvaluation(void) const
{
    int eval = 0;

    for (const auto &square : board_) {
        if (square.isOccupied())
            eval += square.getPiece()->getValue();
    }

    return eval;
}

const BoardSquare & Board::operator[](const Locus &l) const
{
    return board_.at(l);
}

BoardSquare & Board::operator[](const Locus &l)
{
    return board_[l];
}

moveList_t Board::getAllCandidateMoves(void)
{
    moveList_t ret;

    const auto nonCheckCallback = [&](Piece *piece,
                                      const moveList_t &moves)
    {
        auto ourColour = getNextMoveColour();

        for (const auto &move : moves) {
            Mover m(move, *this);
            if (!isInCheck(ourColour))
                ret.push_back(move);
        }

        return true;
    };

    const auto checkCallback = [&](Piece *piece,
                                   const moveList_t &moves)
    {
        auto ourColour = getNextMoveColour();

        for (const auto &move : moves) {
            Mover m(move, *this);
            if (!isInCheck(ourColour))
                ret.push_back(move);
        }

        return true;
    };

    if (isInCheck(getNextMoveColour()))
        forEachPieceMoves(getNextMoveColour(), checkCallback);
    else
        forEachPieceMoves(getNextMoveColour(), nonCheckCallback);

    return ret;
}

const Locus &Board::getKingLocus(Colour c) const
{
    return kingLocus_[static_cast<int>(c)];
}

Locus &Board::getKingLocus(Colour c)
{
    return kingLocus_[static_cast<int>(c)];
}

const CastlingRights Board::getCastlingRights(void) const
{
    return castlingRights_;
}

CastlingRights &Board::getCastlingRights(void)
{
    return castlingRights_;
}

const Locus &Board::getEnPassantLocus(void) const
{
    return enPassantCapture_;
}

Locus &Board::getEnPassantLocus(void)
{
    return enPassantCapture_;
}

int Board::perft(int depth)
{
    if (!depth)
        return 1;

    int nodes = 0;

    for (const auto move : getAllCandidateMoves()) {
        Mover m(move, *this);

        nodes += perft(depth - 1);
    }

    return nodes;
}

void Board::forEachPieceMoves(Colour c, moveCallback_t callback) const
{
    for (const auto &square : board_) {
        if (square.isOccupied() &&
            square.getPiece()->getColour() == c) {
            const auto pieceMoves = square.getPiece()->getCandidateMoves(*this,
                                                                         square.getLocus());
            if (!callback(square.getPiece(), pieceMoves))
                return;
        }
    }
}

Board Board::getStartingBoard()
{
    return constructFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

Board Board::constructFromFEN(std::string fen)
{
    std::vector<std::string> fields;

    boost::split(fields, fen, boost::is_any_of(" "));

    if (fields.size() != 6)
        throw std::invalid_argument("Wrong number of fields in FEN string");

    char turnChar = std::tolower(fields[1][0]);
    Colour turn;

    switch (turnChar)
    {
    case 'w':
        turn = Colour::WHITE;
        break;
    case 'b':
        turn = Colour::BLACK;
        break;
    default:
        throw std::invalid_argument(fields[2] + " is not a valid turn specifier");
    }

    Board board(turn);

    auto enPassantSquare = fields[3];

    if (enPassantSquare != "-") {
        Locus enPassantLocus(enPassantSquare[1], enPassantSquare[0]);

        board.getEnPassantLocus() = enPassantLocus;
    }

    std::vector<std::string> boardSpec;
    boost::split(boardSpec, fields[0], boost::is_any_of("/"));

    if (boardSpec.size() != 8)
        throw std::invalid_argument("Board spec does not have 8 fields");

    auto numericRegex = std::regex("[1-8]");
    auto rank = Rank::EIGHT;

    for (const auto boardRow : boardSpec) {
        auto file = File::A;

        for (const auto pieceSpec : boardRow) {
            auto pieceSpecString = std::string(1, pieceSpec);
            auto colour = std::isupper(pieceSpec) ? Colour::WHITE : Colour::BLACK;

            if (std::regex_match(pieceSpecString, numericRegex)) {
                file = static_cast<File>(static_cast<int>(file) +
                                         std::stoi(pieceSpecString));

                continue;
            }

            switch (std::tolower(pieceSpec)) {
            case 'p':
                board[Locus(rank, file)].setPiece(new Pawn(colour));
                break;
            case 'r':
                board[Locus(rank, file)].setPiece(new Rook(colour));
                break;
            case 'n':
                board[Locus(rank, file)].setPiece(new Knight(colour));
                break;
            case 'b':
                board[Locus(rank, file)].setPiece(new Bishop(colour));
                break;
            case 'q':
                board[Locus(rank, file)].setPiece(new Queen(colour));
                break;
            case 'k':
                board[Locus(rank, file)].setPiece(new King(colour));
                board.getKingLocus(colour) = Locus(rank, file);
                break;
            default:
                throw std::invalid_argument("Encountered unknown piece specification: " +
                                            pieceSpecString);
            }

            file = static_cast<File>(static_cast<int>(file) + 1);
        }

        rank = static_cast<Rank>(static_cast<int>(rank) - 0x10);
    }

    return board;
}
