#include <iostream>

#include "uci.h"

int main()
{
    UCI uci(std::cin, std::cout);

    uci.mainLoop();

    return 0;
}
