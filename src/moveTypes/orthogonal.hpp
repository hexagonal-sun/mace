#pragma once

#include <array>
#include "../direction.hpp"

constexpr std::array<Direction, 4> orthogonalMoves {
    Direction::NORTH(),
    Direction::EAST(),
    Direction::SOUTH(),
    Direction::WEST()
};
