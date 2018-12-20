#pragma once

class Locus;

class Direction
{
    friend class Locus;
public:
    Direction()
        : v_(0)
        {
        }

    constexpr Direction operator+(const Direction &other) const
        {
            return Direction(v_ + other.v_);
        }

    void operator+=(const Direction &other)
        {
            v_ += other.v_;
        }

    static constexpr Direction NORTH()
        {
            return Direction(0x10);
        }

    static constexpr Direction EAST()
        {
            return Direction(0x1);
        }

    static constexpr Direction SOUTH()
        {
            return Direction(-0x10);
        }

    static constexpr Direction WEST()
        {
            return Direction(-0x1);
        }
private:
    constexpr Direction(int v)
        :v_(v)
        {}

    int v_;
};
