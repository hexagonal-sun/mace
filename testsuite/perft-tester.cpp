#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "board.h"

static std::string readPerftFile(std::string filename)
{
    std::ifstream perftFile(filename);
    std::string ret;

    std::getline(perftFile, ret);

    return ret;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        std::cerr << "Unexpected number of argumntes. Exiting.\n";
        return 1;
    }

    std::vector<std::string> fields;

    const std::string perftSpecifier(readPerftFile(argv[1]));

    boost::split(fields, perftSpecifier, boost::is_any_of(";"));

    if (fields.size() < 2) {
        std::cerr << "Expected at least two fields in the perft string.\n";
        return 1;
    }

    std::cout << "Constructing board: " << fields[0] << "\n";

    Board b = Board::constructFromFEN(fields[0]);

    for (size_t i = 1; i < fields.size(); i++)
    {
        int expectedPerft = std::stoi(fields[i]);
        int actualPerft = b.perft(i, false);

        if (expectedPerft != actualPerft) {
            std::cerr << "At perft(" << i << "): " << expectedPerft
                      << " != " << actualPerft << "\n";
            return 1;
        }

        std::cout << "At perft(" << i << "): " << expectedPerft
                  << " == " << actualPerft << "\n";
    }

    return 0;
}
