
// Minesweeper headers
#include "cell.hpp"
#include "game.hpp"
#include "map.hpp"
#include "cli.hpp"

// External libraries
#include "raylib.h"

// C++ standard library
#include <chrono>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    Map map;

    Map map2 = map;

    Map map3(16, 16, 1, 32);
    Map map4(16, 16, 2, 32);

    Map map5(map3);

    map = map4;

    cout << map << "\n" << map4;

    return 0;
}
