
// Minesweeper headers
#include "cell.hpp"
#include "game.hpp"
#include "map.hpp"
#include "render.hpp"
#include "exceptions.hpp"

// External libraries
#include "raylib.h"

// C++ standard library
#include <chrono>
#include <iostream>

using namespace std;

int main()
{
    try
    {
        Render RenderContext;
        RenderContext.StartRender();
    }
    catch (const InvalidMoveException &e)
    {
        std::cout << e.what() << "\n";
    }
    catch (const GameConfigException &e)
    {
        std::cout << e.what() << "\n";
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << "\n";
    }
    catch (const MinesweeperException &e)
    {
        std::cout << e.what() << "\n";
    }
    catch (...)
    {
        std::cout << "An unknown error occurred.\n";
    }

    return 0;
}
