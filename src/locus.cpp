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
    : loc_(std::make_tuple(Rank::DUMMY, File::DUMMY))
{
}

Locus::Locus(Rank r, File f)
    : loc_(std::make_tuple(r, f))
{
}

Locus::Locus(char rank, char file)
    : loc_(std::make_tuple(rankNames.at(rank),
                           fileNames.at(file)))
{
}

void Locus::print(std::ostream &stream) const
{
    for (auto &filekv : fileNames)
        if (filekv.second == getFile()) {
            stream << filekv.first;
            break;
        }

    for (auto &rankkv : rankNames)
        if (rankkv.second == getRank()) {
            stream << rankkv.first;
            break;
        }
}

Rank Locus::getRank(void) const
{
    return std::get<0>(loc_);
}

File Locus::getFile(void) const
{
    return std::get<1>(loc_);
}

bool Locus::isValid(void) const
{
    if (getRank() == Rank::DUMMY || getFile() == File::DUMMY)
        return false;

    return true;
}

size_t Locus::getIndex(void) const
{
    if (!isValid())
        throw std::invalid_argument("Attempted to get index of dummy locus");

    return static_cast<size_t>(getRank()) + (static_cast<int>(getFile()) << 3);
}

Locus Locus::translate(Direction d) const
{
    auto rank = getRank();
    auto file = getFile();

    switch (d) {
    case Direction::NORTH:
        switch (rank) {
        case Rank::ONE:
            rank = Rank::TWO;
            break;
        case Rank::TWO:
            rank = Rank::THREE;
            break;
        case Rank::THREE:
            rank = Rank::FOUR;
            break;
        case Rank::FOUR:
            rank = Rank::FIVE;
            break;
        case Rank::FIVE:
            rank = Rank::SIX;
            break;
        case Rank::SIX:
            rank = Rank::SEVEN;
            break;
        case Rank::SEVEN:
            rank = Rank::EIGHT;
            break;
        case Rank::EIGHT:
            return Locus(Rank::DUMMY, File::DUMMY);
        case Rank::DUMMY:
            return Locus(Rank::DUMMY, File::DUMMY);
        }
        break;
    case Direction::SOUTH:
        switch (rank) {
        case Rank::ONE:
            return Locus(Rank::DUMMY, File::DUMMY);
            break;
        case Rank::TWO:
            rank = Rank::ONE;
            break;
        case Rank::THREE:
            rank = Rank::TWO;
            break;
        case Rank::FOUR:
            rank = Rank::THREE;
            break;
        case Rank::FIVE:
            rank = Rank::FOUR;
            break;
        case Rank::SIX:
            rank = Rank::FIVE;
            break;
        case Rank::SEVEN:
            rank = Rank::SIX;
            break;
        case Rank::EIGHT:
            rank = Rank::SEVEN;
            break;
        case Rank::DUMMY:
            return Locus(Rank::DUMMY, File::DUMMY);
        }
        break;
    case Direction::EAST:
        switch (file) {
        case File::A:
            file = File::B;
            break;
        case File::B:
            file = File::C;
            break;
        case File::C:
            file = File::D;
            break;
        case File::D:
            file = File::E;
            break;
        case File::E:
            file = File::F;
            break;
        case File::F:
            file = File::G;
            break;
        case File::G:
            file = File::H;
            break;
        case File::H:
            return Locus(Rank::DUMMY, File::DUMMY);
        case File::DUMMY:
            return Locus(Rank::DUMMY, File::DUMMY);
        }
        break;
    case Direction::WEST:
        switch(file) {
        case File::A:
            return Locus(Rank::DUMMY, File::DUMMY);
            break;
        case File::B:
            file = File::A;
            break;
        case File::C:
            file = File::B;
            break;
        case File::D:
            file = File::C;
            break;
        case File::E:
            file = File::D;
            break;
        case File::F:
            file = File::E;
            break;
        case File::G:
            file = File::F;
            break;
        case File::H:
            file = File::G;
            break;
        case File::DUMMY:
            return Locus(Rank::DUMMY, File::DUMMY);
        }
        break;
    }

    return Locus(rank, file);
}

bool Locus::operator==(const Locus &other) const
{
    return (other.getRank() == getRank()) &&
        (other.getFile() == getFile());
}

std::ostream &operator<<(std::ostream &os, const Locus &l)
{
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
