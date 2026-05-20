#pragma once

// Minesweeper headers
#include "cell.hpp"
#include "map.hpp"

// C++ standard library
#include <chrono>
#include <list>
#include <queue>
#include <vector>

class BasePowerUp;

class Game
{
public:
    enum class GameState
    {
        MENU,
        NOT_STARTED,
        IN_PROGRESS,
        WON,
        LOST
    };

    Game();
    Game(GameState gameState);
    Game(GameState gameState, Map gameMap);
    Game(GameState gameState, Map gameMap, unsigned int flagsPlaced, unsigned int cellsRevealed, unsigned int clicks, std::chrono::steady_clock::time_point startTime);
    Game(const Game &game);
    ~Game();

    void startGame();
    void reset();

    void setMap(const Map &map);

    void revealCell(int x, int y);
    void fillReveal(int x, int y);
    void adjacentReveal(int x, int y);
    void revealFullMap();
    void toggleFlag(int x, int y);

    GameState getCurrentState() const;
    const Map &getMap() const;
    Map &getMap();

    std::chrono::duration<int, std::milli> getElapsedTime() const;
    std::chrono::steady_clock::time_point getStartTime() const;

    unsigned int getClicks() const;
    unsigned int getCellsRevealed() const;
    unsigned int getFlagsPlaced() const;

    // Power-up methods
    void setInvulnerable(bool value);
    bool isInvulnerable() const;
    void consumeShield();
    void activateRadar(int x, int y, int radius);

    void addPowerUpToHistory(BasePowerUp *p);
    unsigned int getPowerUpCount(const std::string &type = "all") const;
    void showPowerUpHistory() const;

    // Operator de atribuire
    Game &operator=(const Game &other);

    // Operatori de comparatie
    bool operator==(const Game &other) const;
    bool operator!=(const Game &other) const;

    bool operator>=(const Game &other) const;
    bool operator<=(const Game &other) const;
    bool operator>(const Game &other) const;
    bool operator<(const Game &other) const;

    // Operatori aritmetici
    Game &operator+(unsigned int bombs);
    Game &operator*(double factor);
    Game &operator++();

    // Operator []
    Cell &operator[](int index);

    // Friend pentru ostream, istream
    friend std::ostream &operator<<(std::ostream &os, const Game &game);
    friend std::istream &operator>>(std::istream &os, Game &game);

private:
    Map gameMap;

    GameState gameState;

    unsigned int flagsPlaced;
    unsigned int cellsRevealed;
    unsigned int clicks;
    std::chrono::steady_clock::time_point startTime;

    bool invulnerable = false;
    std::list<BasePowerUp *> powerUpHistory;

    void clearPowerUpHistory();
    void checkWinCondition();
    void loseGame();
};

std::ostream &operator<<(std::ostream &os, const Game &game);
std::istream &operator>>(std::istream &os, Game &game);
