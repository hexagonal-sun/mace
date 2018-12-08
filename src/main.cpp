#include <iostream>
#include "board.h"

int main()
{
    Board b = Board::getStartingBoard();

    while (1)
    {
        b.printBoard(std::cout);
        b.getAllCandidateMoves();

        std::string from;
        std::string to;
        std::cout << "From: ";
        std::cin >> from;
        if (from == "exit")
            return 0;
        std::cout << "To: ";
        std::cin >> to;

        b = b.move(from, to);
    }

    return 0;
}
