#pragma once
#include <vector>
#include <ostream>
#include <tuple>

enum class Rank {
    ONE   = 0x00,
    TWO   = 0x10,
    THREE = 0x20,
    FOUR  = 0x30,
    FIVE  = 0x40,
    SIX   = 0x50,
    SEVEN = 0x60,
    EIGHT = 0x70,
};

enum class File {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    F = 5,
    G = 6,
    H = 7,
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
    Locus();
    Locus(Rank r, File f);
    Locus(char rank, char file);
    size_t getIndex(void) const;
    Rank getRank(void) const;
    bool isValid(void) const;
    File getFile(void) const;
    Locus translate(Direction d) const;
    bool operator==(const Locus& other) const;
private:
    Locus(int idx);
    uint8_t idx_;
};

std::ostream &operator<<(std::ostream &os, const Locus &l);

inline Locus operator+(Rank rank, File file)
{
    return Locus(rank, file);
}
