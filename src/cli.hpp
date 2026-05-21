#pragma once

// Minesweeper headers
#include "cell.hpp"
#include "game_setup.hpp"
#include "map.hpp"
#include "game.hpp"
#include "stats.hpp"

// C++ Standard libraries
#include <vector>

struct MapSettings
{
    unsigned int width;
    unsigned int height;
    unsigned int bombs;
    unsigned long long seed;

    MapSettings();
    MapSettings(unsigned int w, unsigned int h, unsigned int b, unsigned long long s)
        : width(w), height(h), bombs(b), seed(s) {}

    void generateRandomSeed();
};

class Commandline final
{
public:
    enum class MenuOption
    {
        START_GAME = 1,
        CHANGE_SETTINGS,
        VIEW_STATS,
        VIEW_ALL_PLAYERS,
        VIEW_JOURNAL,
        EXIT
    };

private:
    MapSettings settings;
    Leaderboard leaderboard;

    void printMainMenu() const;
    void printSettingsMenu() const;

    void handleMainMenu();
    void handleSettingsMenu();

    void startNewGame();
    void processGameInput(Game &game);

    void displayMap(const Map &map, int cursorX, int cursorY) const;
    void displayGameStatus(const Game &game, bool withControls) const;

    Match saveGameRecord(const Game &game, bool won);
    void displayStatistics();
    void displayAllPlayersStatistics();
    void displayJournal();

    int readIntInput(const char *prompt, int min, int max);
    unsigned long long readULLInput(const char *prompt);
    void readStringInput(const char *prompt, char *buffer, size_t bufferSize);

    void clearScreen() const;
    void waitForEnter() const;

    void enableRawMode();
    void disableRawMode();
    void generateRandomSeed();

public:
    Commandline();
    ~Commandline();

    void StartCommandline();
};
