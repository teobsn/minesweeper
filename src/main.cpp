#include "cli.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#include <iostream>

int main()
{
    // ThemeManager e un singleton Meyers, deci nu poate fi declarat aici
    //
    // ThemeManager copy = ThemeManager::getInstance(); // error: copy constructor deleted
    // ThemeManager other; // error: default constructor is private
    try
    {
        Commandline cli;
        cli.StartCommandline();
    }
    catch (const MinesweeperException &e)
    {
        Logger::getInstance().error(e.what());
        std::cout << e.what() << "\n";
    }
    catch (...)
    {
        Logger::getInstance().error("Unknown error in CLI entry point");
        std::cout << "An unknown error occurred.\n";
    }

    return 0;
}
