
#include "cli.hpp"
#include "exceptions.hpp"

int main()
{
    try
    {
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
        std::cout << "A aparut o eroare necunoscuta.\n";
    }
    return 0;
}
