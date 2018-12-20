#pragma once

#include <array>
#include "direction.hpp"

constexpr std::array orthogonalMoves {
    Direction::NORTH(),
    Direction::EAST(),
    Direction::SOUTH(),
    Direction::WEST()
};

constexpr std::array diagonalMoves {
    Direction::NORTH() + Direction::EAST(),
    Direction::NORTH() + Direction::WEST(),
    Direction::SOUTH() + Direction::EAST(),
    Direction::SOUTH() + Direction::WEST()
};

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

constexpr std::array knightMoves {
    Direction::NORTH() + Direction::NORTH() + Direction::EAST(),
    Direction::NORTH() + Direction::NORTH() + Direction::WEST(),
    Direction::EAST() + Direction::EAST() + Direction::NORTH(),
    Direction::EAST() + Direction::EAST() + Direction::SOUTH(),
    Direction::SOUTH() + Direction::SOUTH() + Direction::EAST(),
    Direction::SOUTH() + Direction::SOUTH() + Direction::WEST(),
    Direction::WEST() + Direction::WEST() + Direction::NORTH(),
    Direction::WEST() + Direction::WEST() + Direction::SOUTH(),
};
