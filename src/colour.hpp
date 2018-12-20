#pragma once

#include <map>

enum class Colour
{
    WHITE = 0,
    BLACK = 1
};

static const std::map<Colour, std::string> colourNames =
{
    {Colour::WHITE, "White"},
    {Colour::BLACK, "Black"}
};

inline Colour getOppositeColour(Colour col)
{
    if (col == Colour::WHITE)
        return Colour::BLACK;
    else
        return Colour::WHITE;
}

inline Colour operator~(Colour c)
{
    return getOppositeColour(c);
}
