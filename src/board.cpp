#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <locale>
#include <numeric>
#include <iostream>
#include <regex>

#include "board.h"
#include "zobrist.h"
#include "mover.hpp"
#include "moveGen.h"
#include "moveTypes/orthogonal.hpp"
#include "moveTypes/diagonal.hpp"
#include "moveTypes/knight.hpp"

Board::Board(Colour nextMoveColour)
    : nextMoveColour_(nextMoveColour)
{
    for (const auto &pt : {PieceType::BISHOP,
                          PieceType::KING,
                          PieceType::KNIGHT,
                          PieceType::PAWN,
                          PieceType::QUEEN,
                          PieceType::ROOK})
        pieceCounts[pt] = 0;
}

const SquareState & Board::getSquare(std::string name) const
{
    if (name.length() != 2)
        throw std::invalid_argument("square specifier of incorrect format");

    char rank = name[1];
    char file = name[0];

    return board_.at(Locus(rank, file));
}

Move Board::validateMove(std::string from, std::string to,
                         const MoveType promotion)
{
    return validateMove(Locus(from[1], from[0]), Locus(to[1], to[0]),
                        promotion);
}

const Colour Board::getNextMoveColour(void) const
{
    return nextMoveColour_;
}

Colour & Board::getNextMoveColour(void)
{
    return nextMoveColour_;
}

Move Board::validateMove(const Locus & from, const Locus &to,
                         const MoveType promotion)
{
    const auto &allowedMoves = MoveGen::getLegalMoves(*this);

    for (const auto &m : allowedMoves) {
        if (m.getFrom() == from && m.getTo() == to) {
            if (!isPromotion(promotion))
                return m;

            if (m.getType() == promotion)
                return m;
        }
    }

    return Move();
}

void Board::printBoard(std::ostream &stream) const
{
    for (const auto &rank : RANKS)
    {
        for (const auto &file : FILES) {
            stream << board_.at(Locus(rank, file));

            stream << " ";
        }

        stream << "\n";
    }

    stream << "To move: " << colourNames.at(nextMoveColour_) << "\n";
    stream << "Zobrist Hash: 0x" << std::hex << zobHash.getHash(*this) << "\n";
}

locusList_t Board::locatePiece(Colour c, PieceType t) const
{
    locusList_t ret;

    for (const auto [square, loc] : board_) {
        if (square.isOccupied() &&
            square.getColour() == c &&
            square.getPieceType() == t)
            ret.push_back(loc);
    }

    return ret;
}

bool Board::isSquareUnderAttack(Locus l, Colour attackingColour) const
{
    for (const auto &orthoDirection : orthogonalMoves) {

        int distance = 1;

        for (const auto &sq : board_.getRayIterator(l, orthoDirection)) {
            if (sq.isOccupied()) {
                auto type = sq.getPieceType();

                if (sq.getColour() == attackingColour &&
                    (type == PieceType::QUEEN ||
                     type == PieceType::ROOK  ||
                     (type == PieceType::KING && distance == 1)))
                    return true;

                break;
            }

            distance++;
        }
    }

    for (const auto &diagDirection : diagonalMoves) {

        int distance = 1;

        for (const auto &sq : board_.getRayIterator(l, diagDirection)) {
            if (sq.isOccupied()) {
                auto type = sq.getPieceType();

                // This is the direction (y-component) that this
                // square can be attacked from by a pawn.  Notice that
                // this component depends upon the colour of the pawn.
                auto pawnTakeDirection = attackingColour == Colour::WHITE ?
                    Direction::SOUTH() : Direction::NORTH();

                if (sq.getColour() == attackingColour &&
                    (type == PieceType::QUEEN            ||
                     type == PieceType::BISHOP           ||
                     (type == PieceType::KING && distance == 1) ||
                     (type == PieceType::PAWN && distance == 1
                      && (diagDirection == pawnTakeDirection + Direction::EAST() ||
                          diagDirection == pawnTakeDirection + Direction::WEST()))))
                    return true;

                break;
            }
            distance++;
        }
    }

    for (const auto &knightDir : knightMoves)
    {
        const auto knightLoc = l + knightDir;

        if (!knightLoc.isValid())
            continue;

        const auto &sq = board_[knightLoc];

        if (sq.isOccupied() &&
            sq.getColour() == attackingColour &&
            sq.getPieceType() == PieceType::KNIGHT)
            return true;
    }

    return false;
}

const bool Board::isInCheck(Colour kingsColour) const
{
    const auto &kingLocus = getKingLocus(kingsColour);

    return isSquareUnderAttack(kingLocus,
                               getOppositeColour(kingsColour));
}

const SquareState & Board::operator[](const Locus &l) const
{
    return board_.at(l);
}

SquareState & Board::operator[](const Locus &l)
{
    return board_[l];
}

bool Board::isDraw() const
{
    auto totalPieces = std::accumulate(pieceCounts.begin(),
                                       pieceCounts.end(),
                                       0,
                                       [](size_t cur, auto &elm)
        {
            return cur + elm.second;
        });

    // By definition if there are only two pieces remaining then they
    // have to be kings, which is a draw in insufficient material.
    if (totalPieces == 2)
        return true;

    if (totalPieces == 3 &&
        (pieceCounts.at(PieceType::BISHOP) == 1 ||
         pieceCounts.at(PieceType::KNIGHT) == 1))
        return true;

    return false;
}

ChessBoard &Board::getChessBoard()
{
    return board_;
}

const ChessBoard &Board::getChessBoard() const
{
    return board_;
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

int Board::perft(int depth, bool divide)
{
    if (!depth)
        return 1;

    int nodes = 0;

    for (const auto &move : MoveGen::getLegalMoves(*this)) {
        Mover<MoverType::REVERT> m(move, *this);

        int moveNodes = perft(depth - 1, false);

        if (divide)
            std::cout << move << ": " << moveNodes << "\n";

        nodes += moveNodes;
    }

    return nodes;
}

Board Board::getStartingBoard()
{
    return constructFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void Board::printFEN(std::ostream &os) const
{
    for (const auto &rank : RANKS) {
        size_t numEmptyFiles = 0;

        for (const auto &file : FILES)
            if (board_[rank + file].isOccupied()) {
                if (numEmptyFiles)
                    os << numEmptyFiles;

                os << board_[rank + file];
                numEmptyFiles = 0;

            } else
                numEmptyFiles++;

        if (numEmptyFiles)
            os << numEmptyFiles;

        os << "/";
    }

    os << " ";
    os << (getNextMoveColour() == Colour::WHITE ? "w" : "b");
    os << " ";

    bool havePrintedRights = false;

    const auto cr = getCastlingRights();

    for (const Colour &c : {Colour::WHITE, Colour::BLACK})
        for (const auto &kqMask : {getKingSideMask(), getQueenSideMask()})
            if ((cr & getCastlingMask(c) & kqMask).any()) {
                auto pchar = (kqMask == getKingSideMask() ? 'k' : 'q');

                os << (char)(c == Colour::WHITE ? std::toupper(pchar) : pchar);

                havePrintedRights = true;
            }

    if (!havePrintedRights)
        os << "- ";
    else
        os << " ";

    if (getEnPassantLocus().isValid())
        os << getEnPassantLocus();
    else
        os << "-";

    os << " 0 1\n";
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
    CastlingRights cr;

    if (fields[2] == "-")
        cr.reset();
    else
        for (const auto &castlingField : fields[2])
            switch (castlingField) {
            case 'K':
                cr.set(WhiteKingSideBit);
                break;
            case 'Q':
                cr.set(WhiteQueenSideBit);
                break;
            case 'k':
                cr.set(BlackKingSideBit);
                break;
            case 'q':
                cr.set(BlackQueenSideBit);
                break;
            default:
                throw std::invalid_argument("Castling rights specifier has unexpected char");
            }

    board.getCastlingRights() = cr;

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

    for (const auto &boardRow : boardSpec) {
        auto file = File::A;

        for (const auto &pieceSpec : boardRow) {
            auto pieceSpecString = std::string(1, pieceSpec);
            auto colour = std::isupper(pieceSpec) ? Colour::WHITE : Colour::BLACK;

            if (std::regex_match(pieceSpecString, numericRegex)) {
                file = static_cast<File>(static_cast<int>(file) +
                                         std::stoi(pieceSpecString));

                continue;
            }

            switch (std::tolower(pieceSpec)) {
            case 'p':
                board[Locus(rank, file)] = PieceType::PAWN + colour;
                break;
            case 'r':
                board[Locus(rank, file)] = PieceType::ROOK + colour;
                break;
            case 'n':
                board[Locus(rank, file)] = PieceType::KNIGHT + colour;
                break;
            case 'b':
                board[Locus(rank, file)] = PieceType::BISHOP + colour;
                break;
            case 'q':
                board[Locus(rank, file)] = PieceType::QUEEN + colour;
                break;
            case 'k':
                board[Locus(rank, file)] = PieceType::KING + colour;
                board.getKingLocus(colour) = Locus(rank, file);
                break;
            default:
                throw std::invalid_argument("Encountered unknown piece specification: " +
                                            pieceSpecString);
            }

            board.pieceCounts[board[rank + file].getPieceType()]++;

            file = static_cast<File>(static_cast<int>(file) + 1);
        }

        rank = static_cast<Rank>(static_cast<int>(rank) - 0x10);
    }

    return board;
}
