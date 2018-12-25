#pragma once

#include <array>
#include "../direction.hpp"

constexpr std::array<Direction, 8> knightMoves {
    Direction::NORTH() + Direction::NORTH() + Direction::EAST(),
    Direction::NORTH() + Direction::NORTH() + Direction::WEST(),
    Direction::EAST() + Direction::EAST() + Direction::NORTH(),
    Direction::EAST() + Direction::EAST() + Direction::SOUTH(),
    Direction::SOUTH() + Direction::SOUTH() + Direction::EAST(),
    Direction::SOUTH() + Direction::SOUTH() + Direction::WEST(),
    Direction::WEST() + Direction::WEST() + Direction::NORTH(),
    Direction::WEST() + Direction::WEST() + Direction::SOUTH(),
};
