#include <iostream>
#include "board.h"
#include "search.h"
#include "mover.hpp"

int main()
{
    Board b = Board::getStartingBoard();

    while (1)
    {
        b.printBoard(std::cout);

        std::string from;
        std::string to;
        std::cout << "From: ";
        std::cin >> from;
        if (from == "exit")
            return 0;
        std::cout << "To: ";
        std::cin >> to;
        std::cout << "\n";

        if (!b.validateMove(from, to)) {
            std::cout << "Invalid move\n";
            continue;
        }

        Move m = {Locus(from[1], from[0]), Locus(to[1], to[0])};
        Mover a(m, b);

        std::cout << "Thinking....\n";

        Move bestMove = searchMove(b, 5);

        std::cout << "My move: " << std::get<0>(bestMove) << std::get<1>(bestMove) << "\n";
    }

    return 0;
}
