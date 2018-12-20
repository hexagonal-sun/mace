#include <iostream>
#include "board.h"
#include "piece.h"
#include "search.h"
#include "mover.hpp"

void play(Board &b)
{
    b.printBoard(std::cout);

    for (const auto move : b.getAllCandidateMoves())
        std::cout << "Move: " << move << "\n";

    std::string from;
    std::string to;
    std::cout << "From: ";
    std::cin >> from;
    if (from == "exit")
        return;
    std::cout << "To: ";
    std::cin >> to;
    std::cout << "\n";

    auto m = b.validateMove(from, to);

    if (!m.isValid()) {
        std::cout << "Invalid move\n";
        play(b);
    }

    Mover a(m, b);

    play(b);
}

int main()
{
    Board b = Board::getStartingBoard();

    play(b);

    return 0;
}
