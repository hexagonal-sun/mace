#include <iostream>
#include "board.h"
#include "piece.h"
#include "mover.hpp"

void doPerftSplit(Board &b, size_t perftSplit)
{
    b.printBoard(std::cout);
    b.printFEN(std::cout);
    b.perft(perftSplit, true);

    if (perftSplit == 1)
        return;

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
        doPerftSplit(b, perftSplit);
    }

    Mover a(m, b);

    doPerftSplit(b, perftSplit - 1);
}

int main()
{
    std::string fen;
    size_t perftDepth;

    std::cout << "Enter FEN string: ";
    std::getline(std::cin, fen);

    std::cout << "Constructing " << fen;
    Board b = Board::constructFromFEN(fen);

    std::cout << "Done.\nEnter Perft Depth: ";
    std::cin >> perftDepth;

    std::cout << "Starting\n";

    doPerftSplit(b, perftDepth);

    return 0;
}
