#include <iostream>

#include "game.hpp"

int main(int argc, char *argv[])
{
    Game game(argc, argv);

    if (game.Init())
    {
        game.Run();
        game.End();
    }

    return 0;
}
