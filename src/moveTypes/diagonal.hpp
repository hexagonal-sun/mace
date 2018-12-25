#pragma once

#include <array>
#include "../direction.hpp"

constexpr std::array<Direction, 4> diagonalMoves {
    Direction::NORTH() + Direction::EAST(),
    Direction::NORTH() + Direction::WEST(),
    Direction::SOUTH() + Direction::EAST(),
    Direction::SOUTH() + Direction::WEST()
};
