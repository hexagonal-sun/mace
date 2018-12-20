#pragma once

#include <array>
#include "../direction.hpp"

constexpr std::array orthoDiagonalMoves {
    Direction::NORTH(),
    Direction::EAST(),
    Direction::SOUTH(),
    Direction::WEST(),
    Direction::NORTH() + Direction::EAST(),
    Direction::NORTH() + Direction::WEST(),
    Direction::SOUTH() + Direction::EAST(),
    Direction::SOUTH() + Direction::WEST()
};
