#include <boost/algorithm/string.hpp>
#include <string>
#include <exception>

#include "search.h"
#include "mover.hpp"
#include "uci.h"
#include "mcts.h"
#include "config.h"

void UCI::handleCommandUCI() const
{
    os_ << "id name " << PACKAGE_NAME << "\n";
    os_ << "id author " << "Matthew J. Leach" << "\n";
    os_ << "\n";
    os_ << "uciok\n";
}

void UCI::handleCommandIsReady() const
{
    os_ << "readyok\n";
}

void UCI::handleCommandUCINewGame()
{
    b_ = Board::getStartingBoard();
}

void UCI::handleCommandPosition(std::string line)
{
    std::vector<std::string> v;
    b_ = Board::getStartingBoard();

    boost::split(v, line, boost::is_any_of(" "));
    bool expectMove = false;

    if (v[1] == "fen") {
        std::string fenString = "";
        for (size_t i = 2; i < (2 + 6); i++)
            fenString += v[i] + " ";

        b_ = Board::constructFromFEN(fenString);
    }

    for (auto s : v) {
        if (expectMove) {
            if (s.size() != 4)
                throw std::invalid_argument("Unknown move specification: '" +
                                            s + "'");

            auto from = s.substr(0, 2);
            auto to = s.substr(2);

            auto move = b_.validateMove(from, to);

            if (!move.isValid())
                throw std::invalid_argument("Illegal move: '" + s + "'");

            Mover<MoverType::COMMIT> m(move, b_);
        }

        if (s == "moves")
            expectMove = true;
    }
}

void UCI::dumpResults(SearchResults &results)
{
    auto score = results.getScore();

    if (b_.getNextMoveColour() == Colour::BLACK)
        score = -score;

    os_ << "info depth " << results.getDepth()
        << " nps " << results.getNPS()
        << " nodes " << results.getNodes()
        << " time " << results.getDuration<std::chrono::milliseconds>()
        << " score cp " << score
        << " pv " << results.getBestMove();

    // for (const auto move : results.getPV())
    //     os_ << move << " ";

    os_ << std::endl;
}

void UCI::handleCommandGo(std::string line)
{
    auto dumpResultsCallback = std::bind(&UCI::dumpResults, this,
                                         std::placeholders::_1);

    auto move = searchMove(b_, std::chrono::milliseconds(10),
                           dumpResultsCallback);

    os_ << "bestmove " <<  move << "\n";
}

void UCI::mainLoop()
{
    while (1)
    {
        std::string line;
        std::string pos("position");
        std::string go("go");

        std::getline(is_, line);

        if (line == "uci")
            handleCommandUCI();
        else if (line == "isready")
            handleCommandIsReady();
        else if (line == "ucinewgame")
            handleCommandUCINewGame();
        else if(line.compare(0, pos.length(), pos) == 0)
            handleCommandPosition(line);
        else if(line.compare(0, go.length(), go) == 0)
            handleCommandGo(line);
        else
            throw std::invalid_argument("Unknown UCI command " + line);
    }
}
