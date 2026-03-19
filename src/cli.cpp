
// Minesweeper headers
#include "cli.hpp"

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
        std::cout << "WASD: Move | ENTER: Reveal | SPACE: Mark/Flag | E: Chording | Q: Quit\n";
}

void Commandline::startNewGame()
{
    clearScreen();

    // Harta, joc nou
    Map gameMap(settings.width, settings.height, settings.seed, settings.bombs);
    Game game(Game::GameState::NOT_STARTED, gameMap);

    // Game Loop
    processGameInput(game);

    // Sfarsit game loop -> salvam "meciul"
    bool won = game.getCurrentState() == Game::GameState::WON;

    std::cout << "\n";
    if (won)
        std::cout << "You won!\n";
    else
        std::cout << "You lost! Game over!\n\n";

    // Cream meciul (statistica) din jocul jucat
    Match match = saveGameRecord(game, won);

    std::cout << "Time: " << match.getTimePlayed().count() / 1000 << "s\n";
    std::cout << "Clicks: " << match.getClicks() << "\n";
    std::cout.flush();

    char playerName[64] = "";
    std::cout << "\nEnter your name: ";

    std::cin.getline(playerName, 64);

    PlayerStats *player = nullptr;
    if (playerName[0] != '\0')
        player = Leaderboard::addPlayer(playerName);
    
    if (player)
    {
        player->addMatch(match);
        Leaderboard::addGame();
    }

    generateRandomSeed();

    std::cout << "\n";
    waitForEnter();
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

void Commandline::displayStatistics() const
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

    const Match *matches = player->getMatches();
    int totalGames = player->getMatchesNumber();
    unsigned int wins = player->getWins();
    unsigned int totalTime = 0;
    unsigned int totalClicks = 0;

    for (int i = 0; i < totalGames; ++i)
    {
        totalTime += matches[i].getTimePlayed().count();
        totalClicks += matches[i].getClicks();
    }

    float winRate = player->getWinRate() * 100.0f;

    std::cout << "Games: " << totalGames << "\n";
    std::cout << "Wins: " << wins << " | Losses: " << (totalGames - wins) << "\n";
    std::cout << "Win rate: " << std::fixed << std::setprecision(1) << winRate << "%\n";
    std::cout << "Total time: " << (totalTime / 1000) << "s\n";
    std::cout << "Average number of clicks: " << (totalGames > 0 ? totalClicks / static_cast<unsigned int>(totalGames) : 0) << "\n";

    const char *name = player->getPlayerName();
    if (name != nullptr)
        std::cout << "\nPlayer: " << name << "\n";

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

    std::cout << "\n";
    waitForEnter();
}

void Commandline::displayAllPlayersStatistics() const
{
    clearScreen();

    std::cout << "All players statistics:\n\n";

    unsigned int playerCount = Leaderboard::getPlayerCount();

    if (playerCount == 0)
    {
        std::cout << "No players yet. Play some games first!\n";
        waitForEnter();
        return;
    }

    for (unsigned int i = 0; i < playerCount; ++i)
    {
        PlayerStats *player = Leaderboard::getPlayer(i);
        if (!player)
            continue;

        const char *playerName = player->getPlayerName();
        int totalGames = player->getMatchesNumber();
        float winRate = player->getWinRate() * 100.0f;

        std::cout << "Player: " << (playerName ? playerName : "Unknown") << "\n";
        std::cout << "  Games: " << totalGames << "\n";
        std::cout << "  Win Rate: " << std::fixed << std::setprecision(1) << winRate << "%\n";

        if (totalGames > 0)
        {
            const Match *matches = player->getMatches();
            unsigned int totalTime = 0;
            unsigned int totalClicks = 0;
            unsigned int wins = player->getWins();

            for (int j = 0; j < totalGames; ++j)
            {
                totalTime += matches[j].getTimePlayed().count();
                totalClicks += matches[j].getClicks();
            }

            std::cout << "  Wins: " << wins << " | Losses: " << (totalGames - wins) << "\n";
            std::cout << "  Total time: " << (totalTime / 1000) << "s\n";
            std::cout << "  Average number of clicks: " << (totalClicks / static_cast<unsigned int>(totalGames)) << "\n";
        }

        std::cout << "\n";
    }

    std::cout << "Total games played: " << Leaderboard::getOverallGames() << "\n";
    std::cout << "\n";
    waitForEnter();
}

void Commandline::StartCommandline()
{
    while (true)
    {
        clearScreen();
        printMainMenu();
        handleMainMenu();
    }
}
