
// Minesweeper headers
#include "cell.hpp"
#include "game.hpp"
#include "map.hpp"
#include "render.hpp"

// External libraries
#include "raylib.h"

// C++ standard library
#include <chrono>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    Game mainGame;
    Render RenderContext(mainGame);

    RenderContext.StartRender();

    return 0;
}
