#pragma once
#include <vector>
#include <ostream>
#include <tuple>

enum class Rank {
    ONE   = 0,
    TWO   = 1,
    THREE = 2,
    FOUR  = 3,
    FIVE  = 4,
    SIX   = 5,
    SEVEN = 6,
    EIGHT = 7,
    DUMMY
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
    DUMMY
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
    void print(std::ostream &o) const;
    size_t getIndex(void) const;
    Rank getRank(void) const;
    bool isValid(void) const;
    File getFile(void) const;
    Locus translate(Direction d) const;
    bool operator==(const Locus& other) const;
private:
    std::tuple<Rank, File> loc_;
};

std::ostream &operator<<(std::ostream &os, const Locus &l);
