#include <unordered_map>
#include <iostream>

#include "board.h"

using Counts = std::unordered_map<SquareState, int, SquareState::Hash>;

static Counts performCount(Board &b)
{
    Counts ret;

    for (const auto pt : pieceTypes)
        for (const auto colour : {Colour::WHITE, Colour::BLACK})
            ret[pt + colour] = 0;

    for (const auto [sq, loc] : b.getChessBoard())
        if (sq.isOccupied())
            ret[sq]++;

    return ret;
}

static void dumpCounts(Board &b)
{
    Counts ourCount = performCount(b);

    for (const auto [sq, count] : b.pieceCounts)
        if (ourCount.find(sq) == ourCount.end())
            std::cout << "Board has non-existent count: " << sq << count;

    for (const auto [sq, count] : ourCount)
    {
        std::string equality = "==";

        if (count != b.pieceCounts[sq])
            equality = "!=";

        std::cout << count << sq << equality << b.pieceCounts.at(sq) << sq << " ";
    }

    std::cout << std::endl;
}

static void checkCounts(std::string fen)
{
    Board b = Board::constructFromFEN(fen);

    Counts initialCounts = b.pieceCounts;

    if (initialCounts != b.pieceCounts)
    {
        std::cout << "ERROR: Initial piece counts did not match" << std::endl;
        b.printBoard(std::cout);
        dumpCounts(b);
        exit(1);
    }

    for (int i : {1,2,3,4})
    {
        std::cout << "Checking perft " << i << ": ";
        b.perft(i, false, [&](Move m)
        {
            if (performCount(b) != b.pieceCounts)
            {
                std::cout << "ERROR: incorrect count detected after making move " << m << std::endl;
                b.printBoard(std::cout);
                dumpCounts(b);
                exit(1);
            }
        }, [&](Move m)
        {
            if (performCount(b) != b.pieceCounts)
            {
                std::cout << "ERROR: incorrect count detected after unmaking move " << m << std::endl;
                b.printBoard(std::cout);
                dumpCounts(b);
                exit(1);
            }
        });

        if (b.pieceCounts != initialCounts) {
            std::cout << "Error: initial counts did not match counts after perft" << std::endl;
            exit(1);
        }

        dumpCounts(b);
    }
}

int main()
{
    for (std::string fen : {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                            "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
                            "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1;",
                            "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
                            "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
                            "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"})
    {
        std::cout << "Testing position: " << fen << std::endl;
        checkCounts(fen);
    }

    return 0;
}
