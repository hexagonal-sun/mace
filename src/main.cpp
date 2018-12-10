#include <iostream>
#include "board.h"
#include "search.h"

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

        b = b.move(from, to);

        std::cout << "Thinking....\n";

        Move m = searchMove(b, 5);

        std::cout << "My move: " << std::get<0>(m) << std::get<1>(m) << "\n";

        b = b.move(std::get<0>(m), std::get<1>(m));
    }

    return 0;
}
