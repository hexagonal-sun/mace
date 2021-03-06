#include <unordered_map>
#include "locus.h"

static const std::unordered_map<char, Rank> rankNames {
    { '1', Rank::ONE },
    { '2', Rank::TWO },
    { '3', Rank::THREE },
    { '4', Rank::FOUR },
    { '5', Rank::FIVE },
    { '6', Rank::SIX },
    { '7', Rank::SEVEN },
    { '8', Rank::EIGHT },
};

static const std::unordered_map<char, File> fileNames {
    { 'a', File::A },
    { 'b', File::B },
    { 'c', File::C },
    { 'd', File::D },
    { 'e', File::E },
    { 'f', File::F },
    { 'g', File::G },
    { 'h', File::H },
};


Locus::Locus()
    : idx_(0xff)
{
}

Locus::Locus(int idx)
    : idx_(idx)
{
}

Locus::Locus(Rank r, File f)
    : idx_(static_cast<uint8_t>(r) | static_cast<uint8_t>(f))
{
}

Locus::Locus(char rank, char file)
    : idx_(static_cast<uint8_t>(rankNames.at(rank)) |
           static_cast<uint8_t>(fileNames.at(file)))
{
}

Rank Locus::getRank(void) const
{
    return static_cast<Rank>(idx_ & 0xf0);
}

File Locus::getFile(void) const
{
    return static_cast<File>(idx_ & 0x0f);
}

bool Locus::isValid(void) const
{
    if (idx_ & 0x88)
        return false;

    return true;
}

size_t Locus::getIndex(void) const
{
    if (!isValid())
        throw std::invalid_argument("Attempted to get index of dummy locus");

    return idx_;
}

size_t Locus::getArrayIndex(void) const
{
    return (getIndex() & 0xf) | ((getIndex() & 0xf0) >> 1);
}

bool Locus::operator==(const Locus &other) const
{
    return other.idx_ == idx_;
}

Locus Locus::operator+(const Direction d) const
{
    return Locus(idx_ + d.v_);
}

Locus &Locus::operator+=(const Direction d)
{
    idx_ += d.v_;

    return *this;
}

std::ostream &operator<<(std::ostream &os, const Locus &l)
{
    if (!l.isValid()) {
        os << "<INVALID>";
        return os;
    }

    for (auto &filekv : fileNames)
        if (filekv.second == l.getFile()) {
            os << filekv.first;
            break;
        }

    for (auto &rankkv : rankNames)
        if (rankkv.second == l.getRank()) {
            os << rankkv.first;
            break;
        }

    return os;
}
