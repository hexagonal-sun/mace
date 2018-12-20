#pragma once

#include <array>
#include "../direction.hpp"

constexpr std::array orthogonalMoves {
    Direction::NORTH(),
    Direction::EAST(),
    Direction::SOUTH(),
    Direction::WEST()
};
