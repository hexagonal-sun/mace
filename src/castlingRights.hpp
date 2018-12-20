#pragma once
#include <bitset>

#include "colour.hpp"

auto constexpr WhiteKingSideBit  = 0;
auto constexpr BlackKingSideBit  = 1;
auto constexpr WhiteQueenSideBit = 2;
auto constexpr BlackQueenSideBit = 3;

typedef std::bitset<4> CastlingRights;

constexpr CastlingRights getCastlingMask(Colour c)
{
    if (c == Colour::WHITE)
        return CastlingRights((1 << WhiteKingSideBit) |
                              (1 << WhiteQueenSideBit));
    else
        return CastlingRights((1 << BlackKingSideBit) |
                              (1 << BlackQueenSideBit));
}

constexpr CastlingRights getKingSideMask()
{
    return CastlingRights((1 << BlackKingSideBit) |
                          (1 << WhiteKingSideBit));
}

constexpr CastlingRights getQueenSideMask()
{
    return CastlingRights((1 << BlackQueenSideBit) |
                          (1 << WhiteQueenSideBit));
}
