#pragma once
#include <bitset>

auto constexpr WhiteKingSideBit  = 0;
auto constexpr BlackKingSideBit  = 1;
auto constexpr WhiteQueenSideBit = 2;
auto constexpr BlackQueenSideBit = 3;

typedef std::bitset<4> CastlingRights;
