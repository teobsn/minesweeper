#include "cell.hpp"
#include "game.hpp"
#include "map.hpp"
#include "exceptions.hpp"
#include "logger.hpp"
#include "render.hpp"

// External libraries
#include "raylib.h"

int main()
{
    try
    {
        Render renderContext;
        renderContext.StartRender();
    }
    catch (const MinesweeperException &e)
    {
        Logger::getInstance().error(e.what());
        std::cout << e.what() << "\n";
    }
    catch (...)
    {
        Logger::getInstance().error("Unknown error in GUI entry point");
        std::cout << "An unknown error occurred.\n";
    }

    return 0;
}
