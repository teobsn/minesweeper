
// Minesweeper headers
#include "cell.hpp"
#include "game.hpp"
#include "map.hpp"
#include "cli.hpp"

// C++ standard library
#include <chrono>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    Map map;
    
    for (int seed = 0; seed < 1000; ++seed)
    {
        map = Map(16, 16, seed, 0);
        for (int i = 0; i < 256; i++)
        {
            map.revealAll();
            // map.updateAdjacentBombs(true);
            cout << map << "\n";
            cout.flush();
            ++map;

            usleep(25000);
        }
    }

    return 0;
}
