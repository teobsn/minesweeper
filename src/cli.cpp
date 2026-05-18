
// Minesweeper headers
#include "cli.hpp"
#include "powerups.hpp"
#include "exceptions.hpp"

// C++ Standard libraries
#include <iostream>
#include <limits>
#include <cstring>
#include <iomanip>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstdlib>
#include <ctime>

MapSettings::MapSettings() : width(8), height(8), bombs(8), seed(0)
{
    generateRandomSeed();
}

void MapSettings::generateRandomSeed()
{
    // Seed rand
    srand(time(nullptr));

    seed = (static_cast<unsigned long long>(std::rand()) << 32) | std::rand();
}

void Commandline::generateRandomSeed()
{
    settings.generateRandomSeed();
}

Commandline::Commandline()
{
}

Commandline::~Commandline()
{
}

void Commandline::clearScreen() const
{
    std::cout << "\033[2J\033[1;1H";
}

void Commandline::waitForEnter() const
{
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void Commandline::enableRawMode()
{
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void Commandline::disableRawMode()
{
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void Commandline::printMainMenu() const
{
    std::cout << "MINESWEEPER\n\n";

    std::cout << "1. Start Game\n";
    std::cout << "2. Change Map Settings\n";
    std::cout << "3. View My Statistics\n";
    std::cout << "4. View All Players Statistics\n";
    std::cout << "5. Exit\n\n";
}

void Commandline::printSettingsMenu() const
{
    std::cout << "Map settings:\n";

    std::cout << "1. Width:  " << settings.width << "\n";
    std::cout << "2. Height: " << settings.height << "\n";
    std::cout << "3. Bombs:  " << settings.bombs << "\n";
    std::cout << "4. Seed:   " << settings.seed << "\n";
    std::cout << "5. Generate Random Seed\n";
    std::cout << "6. Back\n\n";
}

void Commandline::handleMainMenu()
{
    int choice = readIntInput("Your choice: ", 1, 5);

    try
    {
        switch (static_cast<MenuOption>(choice))
        {
        case MenuOption::START_GAME:
            startNewGame();
            break;
        case MenuOption::CHANGE_SETTINGS:
            handleSettingsMenu();
            break;
        case MenuOption::VIEW_STATS:
            displayStatistics();
            break;
        case MenuOption::VIEW_ALL_PLAYERS:
            displayAllPlayersStatistics();
            break;
        case MenuOption::EXIT:
            exit(0);
            break;
        }
    }
    catch (const InvalidMoveException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const GameConfigException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const MinesweeperException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (...)
    {
        std::cout << "A aparut o eroare necunoscuta.\n";
        waitForEnter();
    }
}

void Commandline::handleSettingsMenu()
{
    clearScreen();
    printSettingsMenu();

    int choice = readIntInput("Your choice: ", 1, 6);

    switch (choice)
    {
    case 1:
    {
        int newWidth = readIntInput("Width: ", 1, 128);
        if (static_cast<unsigned int>(newWidth) * static_cast<unsigned int>(settings.height) >= settings.bombs)
            settings.width = static_cast<unsigned int>(newWidth);
        break;
    }
    case 2:
    {
        int newHeight = readIntInput("Height: ", 1, 128);
        if (static_cast<unsigned int>(newHeight) * static_cast<unsigned int>(settings.width) >= settings.bombs)
            settings.height = static_cast<unsigned int>(newHeight);
        break;
    }
    case 3:
    {
        unsigned int maxBombs = settings.width * settings.height;
        int newBombs = readIntInput("Bombs: ", 0, static_cast<int>(maxBombs));
        settings.bombs = static_cast<unsigned int>(newBombs);
        break;
    }
    case 4:
    {
        unsigned long long newSeed = readULLInput("Seed: ");
        settings.seed = newSeed;
        break;
    }
    case 5:
    {
        generateRandomSeed();
        break;
    }
    case 6:
        return;
    }

    handleSettingsMenu();
}

int Commandline::readIntInput(const char *prompt, int min, int max)
{
    std::string line;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, line);

        int value = 0;
        bool valid = !line.empty();
        for (char c : line)
        {
            if (c < '0' || c > '9')
            {
                valid = false;
                break;
            }
            value = value * 10 + (c - '0');
        }

        if (valid && value >= min && value <= max)
            return value;

        std::cout << "Enter a number between " << min << " and " << max << ".\n";
    }
}

unsigned long long Commandline::readULLInput(const char *prompt)
{
    std::string line;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, line);

        unsigned long long value = 0;
        bool valid = !line.empty();

        // Verificam inputul manual - exista doar stoi, nu si stoull :|
        for (char c : line)
        {
            if (c < '0' || c > '9')
            {
                valid = false;
                break;
            }
            value = value * 10 + (c - '0');
        }

        if (valid)
            return value;

        std::cout << "Enter a positive number.\n";
    }
}

void Commandline::readStringInput(const char *prompt, char *buffer, size_t bufferSize)
{
    std::cout << prompt;
    std::cin.getline(buffer, static_cast<int>(bufferSize));
}

void Commandline::displayMap(const Map &map, int cursorX, int cursorY) const
{
    std::string defaultColor = "\e[0;97m"; // White
    std::string bombColor = "\e[41m";      // Red Background

    std::string colorEscape[] = {
        "\e[0;30m", // 0 -> Black
        "\e[0;94m", // 1 -> Blue
        "\e[0;92m", // 2 -> Green
        "\e[0;91m", // 3 -> Red
        "\e[0;35m", // 4 -> Purple
        "\e[0;31m", // 5 -> Dark Red
        "\e[0;96m", // 6 -> Cyan
        "\e[0;32m", // 7 -> Dark Green
        "\e[0;93m"  // 8 -> Yellow
    };

    unsigned int height = map.getHeight();
    unsigned int width = map.getWidth();

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            if ((int)x == cursorX && (int)y == cursorY)
                std::cout << defaultColor << "X" << defaultColor << " ";
            else if (map.isRevealedAt(x, y))
            {
                if (map.isBombAt(x, y))
                    std::cout << bombColor << "*" << defaultColor << " ";
                else
                {
                    unsigned int adjacentBombs = map.getAdjacentBombsAt(x, y);
                    if (adjacentBombs)
                        std::cout << colorEscape[adjacentBombs] << adjacentBombs << defaultColor << " ";
                    else // Daca nu are bombe adiacente, afisam un spatiu gol
                        std::cout << defaultColor << "  ";
                }
            }
            else
            {
                if (map.isFlaggedAt(x, y))
                    std::cout << defaultColor << "P" << defaultColor << " ";
                else
                    std::cout << defaultColor << "■" << defaultColor << " ";
            }
        }
        std::cout << std::endl;
    }
}

void Commandline::displayGameStatus(const Game &game, bool withControls) const
{
    std::cout << "Bombs: " << game.getMap().getBombs() << " | Time: " << game.getElapsedTime().count() / 1000 << "s\n";

    if (withControls)
    {
        std::cout << "WASD: Move | ENTER: Reveal | SPACE: Mark/Flag | E: Chording | Q: Quit\n";
        std::cout << "+: Add Bomb | -: Remove Bomb | *: Multiply Bombs (x1.5)\n";
        std::cout << "1: Shield | 2: Scanner | 3: Ultimate | 4: Bomb Defuser\n";
    }
}

void Commandline::startNewGame()
{
    try
    {
        bool playAgain = false;
        
        // Harta, joc nou
        Map gameMap(settings.width, settings.height, settings.seed, settings.bombs);
        Game game(Game::GameState::NOT_STARTED, gameMap);

        do {
            clearScreen();

            // Game Loop
            processGameInput(game);

            // Sfarsit game loop -> salvam "meciul"
            bool won = game.getCurrentState() == Game::GameState::WON;

            std::cout << "\n";
            if (won)
                std::cout << "You won!\n";
            else if (game.getCurrentState() == Game::GameState::LOST)
                std::cout << "You lost! Game over!\n\n";
            else
                std::cout << "Game quit.\n\n";

            if (game.getCurrentState() == Game::GameState::WON || game.getCurrentState() == Game::GameState::LOST)
            {
                // Cream meciul (statistica) din jocul jucat
                Match match = saveGameRecord(game, won);

                std::cout << "Time: " << match['t'] / 1000 << "s\n";
                std::cout << "Clicks: " << match.getClicks() << "\n";
                std::cout.flush();

                char playerName[64] = "";
                std::cout << "\nEnter your name to save stats (empty to skip): ";

                std::cin.getline(playerName, 64);

                PlayerStats *player = nullptr;
                if (playerName[0] != '\0')
                    player = Leaderboard::addPlayer(playerName);

                if (player)
                {
                    // player->addMatch(match);
                    *player + match;
                    Leaderboard::addGame();
                }
            }

            std::cout << "\nPlay again with same settings?\n";
            std::cout << "1. Yes\n";
            std::cout << "2. No\n";
            int choice = readIntInput("", 1, 2);
            
            if (choice == 1) {
                playAgain = true;
                generateRandomSeed();
                game.reset();
                game.getMap().setSeed(settings.seed);
                game.getMap() + settings.bombs;
            } else {
                playAgain = false;
            }

        } while (playAgain);

        std::cout << "\n";
        waitForEnter();
    }
    catch (const InvalidMoveException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const GameConfigException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const MinesweeperException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (...)
    {
        std::cout << "A aparut o eroare necunoscuta.\n";
        waitForEnter();
    }
}

void Commandline::processGameInput(Game &game)
{
    game.startGame();

    int cursorX = 0;
    int cursorY = 0;

    enableRawMode();

    while (game.getCurrentState() == Game::GameState::IN_PROGRESS)
    {
        clearScreen();
        displayMap(game.getMap(), cursorX, cursorY);
        displayGameStatus(game, true);

        char c = std::cin.get();

        try
        {
            switch (c)
            {
            case 'w':
            case 'W':
                cursorY = std::max(0, cursorY - 1);
                break;
            case 's':
            case 'S':
                cursorY = std::min(static_cast<int>(game.getMap().getHeight()) - 1, cursorY + 1);
                break;
            case 'a':
            case 'A':
                cursorX = std::max(0, cursorX - 1);
                break;
            case 'd':
            case 'D':
                cursorX = std::min(static_cast<int>(game.getMap().getWidth()) - 1, cursorX + 1);
                break;
            case 'e':
            case 'E':
                game.adjacentReveal(cursorX, cursorY); // Chording
                break;
            case '+':
                game.getMap() + 1;
                break;
            case '-':
                game.getMap() - 1;
                break;
            case '*':
                game.setMap(game.getMap() * 1.5);
                break;
            case '1':
            {
                BasePowerUp* p = new ShieldPowerUp(1, 15.0f);
                p->use(game, cursorX, cursorY); // Valid (metoda 'use' este publica)
                break;
            }
            case '2':
            {
                BasePowerUp* p = new ScannerPowerUp(2, 3);
                p->use(game, cursorX, cursorY); // Valid (metoda publica)
                break;
            }
            case '3':
            {
                BasePowerUp* p = new UltimatePowerUp(3, 10.0f, 5);
                p->use(game, cursorX, cursorY);
                break;
            }
            case '4':
            {
                BasePowerUp* p = new BombDefuser(4, 0.5);
                p->use(game, cursorX, cursorY);
                break;
            }
            case '\n':
            case '\r': // Enter
                game.revealCell(cursorX, cursorY);
                break;
            case ' ': // Space
                game.toggleFlag(cursorX, cursorY);
                break;
            case 'q':
            case 'Q':
                disableRawMode();
                return;
            }
        }
        catch (const InvalidMoveException &e)
        {
            disableRawMode();
            clearScreen();
            std::cout << e.what() << "\n";
            waitForEnter();
            enableRawMode();
        }
        catch (const GameConfigException &e)
        {
            disableRawMode();
            clearScreen();
            std::cout << e.what() << "\n";
            waitForEnter();
            enableRawMode();
        }
        catch (const DatabaseException &e)
        {
            disableRawMode();
            clearScreen();
            std::cout << e.what() << "\n";
            waitForEnter();
            enableRawMode();
        }
        catch (const MinesweeperException &e)
        {
            disableRawMode();
            clearScreen();
            std::cout << e.what() << "\n";
            waitForEnter();
            enableRawMode();
        }
        catch (...)
        {
            disableRawMode();
            clearScreen();
            std::cout << "A aparut o eroare necunoscuta.\n";
            waitForEnter();
            enableRawMode();
        }
    }

    clearScreen();
    game.revealFullMap();
    displayMap(game.getMap(), -1, -1);
    displayGameStatus(game, false);

    disableRawMode();
}

Match Commandline::saveGameRecord(const Game &game, bool won)
{
    Match match(settings.width, settings.height, settings.seed, settings.bombs, game.getCellsRevealed(), game.getClicks(), game.getElapsedTime(), won, true);

    return match;
}

void Commandline::displayStatistics()
{
    try
    {
        clearScreen();

        std::cout << "My statistics\n\n";

        char playerName[64] = "";
        std::cout << "Enter your name: ";
        std::cin.getline(playerName, 64);

        clearScreen();

        PlayerStats *player = Leaderboard::findPlayer(playerName);

        if (!player || player->getMatchesNumber() == 0)
        {
            std::cout << "No games found for this player...\n";
            waitForEnter();
            return;
        }

        bool viewing = true;
        while (viewing)
        {
            clearScreen();
            const Match *matches = player->getMatches();
            int totalGames = player->getMatchesNumber();
            unsigned int wins = player->getWins();
            unsigned int totalTime = 0;

            int *clicksArray = new int[totalGames];
            for (int i = 0; i < totalGames; ++i)
            {
                totalTime += matches[i].getTimePlayed().count();
                clicksArray[i] = static_cast<int>(matches[i].getClicks());
            }

            int totalClicks = 0;
            for (int i = 0; i < totalGames; ++i)
                totalClicks += clicksArray[i];
            
            delete[] clicksArray;

            float winRate = player->getWinRate() * 100.0f;

            std::cout << "Player: " << player->getPlayerName() << "\n";
            std::cout << "Games: " << totalGames << "\n";
            std::cout << "Wins: " << wins << " | Losses: " << (totalGames - wins) << "\n";
            std::cout << "Win rate: " << std::fixed << std::setprecision(1) << winRate << "%\n";
            std::cout << "Total time: " << (totalTime / 1000) << "s\n";
            std::cout << "Average number of clicks: " << (totalGames > 0 ? totalClicks / totalGames : 0) << "\n";

            std::cout << "\nRecent Games:\n";
            size_t start = (static_cast<size_t>(totalGames) > 10) ? static_cast<size_t>(totalGames) - 10 : 0;
            for (size_t i = start; i < static_cast<size_t>(totalGames); ++i)
            {
                const Match &record = matches[i];
                std::cout << (record.getWinState() ? "Win" : "Loss") << " | "
                          << record.getMapWidth() << "x" << record.getMapHeight() << " | "
                          << record.getMapBombs() << " bombs | Seed: " << record.getMapSeed() << " | "
                          << (record.getTimePlayed().count() / 1000) << "s | "
                          << record.getClicks() << " clicks\n";
            }

            std::cout << "\nOptions:\n";
            std::cout << "1. Remove Last Match\n";
            std::cout << "2. Back\n";

            int choice = readIntInput("Your choice: ", 1, 2);

            switch (choice)
            {
            case 1:
                --(*player);
                std::cout << "Last match removed.\n";
                if (player->getMatchesNumber() == 0)
                    viewing = false;

                break;
            default:
                viewing = false;
                break;
            }
        }
    }
    catch (const InvalidMoveException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const GameConfigException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const MinesweeperException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (...)
    {
        std::cout << "A aparut o eroare necunoscuta.\n";
        waitForEnter();
    }
}

void Commandline::displayAllPlayersStatistics()
{
    try
    {
        clearScreen();

        std::cout << "All players statistics:\n\n";

        Leaderboard::sort();

        unsigned int playerCount = Leaderboard::getPlayerCount();

        if (playerCount == 0)
        {
            std::cout << "No players yet. Play some games first!\n";
            waitForEnter();
            return;
        }

        std::cout << "Player\t\tGames\tWins\tLosses\tWin Rate\tTime\tAverage Clicks\n";

        for (unsigned int i = 0; i < playerCount; ++i)
        {
            PlayerStats *player = Leaderboard::getPlayer(i);
            if (!player)
                continue;

            const char *playerName = player->getPlayerName();
            int totalGames = player->getMatchesNumber();
            float winRate = player->getWinRate() * 100.0f;
            unsigned int wins = player->getWins();
            unsigned int totalTime = 0;
            unsigned int totalClicks = 0;

            if (totalGames > 0)
            {
                const Match *matches = player->getMatches();
                for (int j = 0; j < totalGames; ++j)
                {
                    totalTime += matches[j].getTimePlayed().count();
                    totalClicks += matches[j].getClicks();
                }
            }

            std::cout << (playerName ? playerName : "Unknown") << "\t\t"
                      << totalGames << "\t"
                      << wins << "\t"
                      << (totalGames - wins) << "\t"
                      << std::fixed << std::setprecision(1) << winRate << "%\t\t"
                      << (totalTime / 1000) << "s\t"
                      << (totalGames > 0 ? (totalClicks / static_cast<unsigned int>(totalGames)) : 0) << "\n";
        }

        std::cout << "\nTotal games played (all players): " << Leaderboard::getOverallGames() << "\n";
        std::cout << "\n";
        waitForEnter();
    }
    catch (const InvalidMoveException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const GameConfigException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (const MinesweeperException &e)
    {
        std::cout << e.what() << "\n";
        waitForEnter();
    }
    catch (...)
    {
        std::cout << "A aparut o eroare necunoscuta.\n";
        waitForEnter();
    }
}

void Commandline::StartCommandline()
{
    try
    {
        ResourceGuard demoGuard("Stack unwinding demo");
        throw MinesweeperException("Demo stack unwinding");
    }
    catch (const MinesweeperException &)
    {
    }

    while (true)
    {
        try
        {
            clearScreen();
            printMainMenu();
            handleMainMenu();
        }
        catch (const InvalidMoveException &e)
        {
            std::cout << e.what() << "\n";
            waitForEnter();
        }
        catch (const GameConfigException &e)
        {
            std::cout << e.what() << "\n";
            waitForEnter();
        }
        catch (const DatabaseException &e)
        {
            std::cout << e.what() << "\n";
            waitForEnter();
        }
        catch (const MinesweeperException &e)
        {
            std::cout << e.what() << "\n";
            waitForEnter();
        }
        catch (...)
        {
            std::cout << "A aparut o eroare necunoscuta.\n";
            waitForEnter();
        }
    }
}
