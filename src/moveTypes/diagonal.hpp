#pragma once

#include <array>
#include "../direction.hpp"

constexpr std::array diagonalMoves {
    Direction::NORTH() + Direction::EAST(),
    Direction::NORTH() + Direction::WEST(),
    Direction::SOUTH() + Direction::EAST(),
    Direction::SOUTH() + Direction::WEST()
};
