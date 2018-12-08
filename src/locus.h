#pragma once
#include <vector>
#include <ostream>
#include <tuple>

enum class Rank {
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT
};

enum class File {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H
};

enum class Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};


static const std::vector<Rank> RANKS =
{
    Rank::EIGHT, Rank::SEVEN, Rank::SIX, Rank::FIVE,
    Rank::FOUR,  Rank::THREE, Rank::TWO, Rank::ONE
};

static const std::vector<File> FILES =
{
    File::A, File::B, File::C, File::D,
    File::E, File::F, File::G, File::H
};

class Locus
{
public:
    Locus(Rank r, File f);
    Locus(char rank, char file);
    void print(std::ostream &o) const;
    Rank getRank(void) const;
    File getFile(void) const;
    Locus translate(Direction d) const;
    bool operator==(const Locus& other) const;
private:
    std::tuple<Rank, File> loc_;
};

std::ostream &operator<<(std::ostream &os, const Locus &l);

namespace std
{
    template <>
    struct hash<Locus>
    {
        size_t operator()(const Locus &locus) const
        {
            return hash<Rank>()(locus.getRank()) &
                hash<File>()(locus.getFile()) << 4;
        }
    };
}
