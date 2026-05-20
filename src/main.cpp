
#include "cli.hpp"
#include "exceptions.hpp"

int main()
{
    try
    {
        // ThemeManager e un singleton Meyers, deci nu poate fi declarat aici 
        //
        // ThemeManager copy = ThemeManager::instance(); // error: copy constructor deleted
        // ThemeManager other;                           // error: default constructor is private in a Meyers singleton
        Commandline cli;
        cli.StartCommandline();
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
