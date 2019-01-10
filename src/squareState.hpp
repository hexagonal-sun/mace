#pragma once

#include <stdint.h>
#include <ostream>

#include "colour.hpp"
#include "pieceType.hpp"

class SquareState
{
public:
    constexpr SquareState()
        : state_(0)
        {}

    explicit SquareState(uint8_t state)
        : state_(state)
        {}

    bool isOccupied() const
        {
            return (state_ & 0xe);
        }
    Colour getColour() const
        {
            return static_cast<Colour>(state_ & 0x1);
        }
    PieceType getPieceType() const
        {
            return static_cast<PieceType>(state_ & 0xe);
        }

    bool operator==(const SquareState& other) const
        {
            return state_ == other.state_;
        }

    static const SquareState UNOCCUPIED;

    struct Hash
    {
        std::size_t operator()(SquareState const& h) const noexcept
            {
                return h.state_;
            }
    };
private:
    uint8_t state_;
};

inline SquareState operator+(const PieceType p, const Colour c)
{
    return SquareState(static_cast<uint8_t>(p) |
                       static_cast<uint8_t>(c));
}

inline std::ostream &operator<<(std::ostream &os, const SquareState &square)
{
    char printChar;

    switch (square.getPieceType())
    {
    case PieceType::UNOCCUPIED:
        printChar = '.';
        break;
    case PieceType::PAWN:
        printChar = 'p';
        break;
    case PieceType::ROOK:
        printChar = 'r';
        break;
    case PieceType::BISHOP:
        printChar = 'b';
        break;
    case PieceType::KNIGHT:
        printChar = 'n';
        break;
    case PieceType::QUEEN:
        printChar = 'q';
        break;
    case PieceType::KING:
        printChar = 'k';
        break;
    }

    if (square.getColour() == Colour::WHITE)
        printChar = std::toupper(printChar);

    os << printChar;

    return os;
}
