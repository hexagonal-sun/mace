#include <iostream>
#include "board.h"

class UCI
{
public:
    UCI(std::istream &is, std::ostream &os)
        : is_(is), os_(os), b_(Board::getStartingBoard())
        {}

    void mainLoop();

private:
    std::istream &is_;
    std::ostream &os_;
    void handleCommandUCI() const;
    void handleCommandIsReady() const;
    void handleCommandUCINewGame();
    void handleCommandPosition(std::string line);
    void handleCommandGo(std::string line);
    Board b_;
};
