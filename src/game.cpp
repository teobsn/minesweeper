
// Minesweeper headers
#include "game.hpp"
#include "cell.hpp"
#include "map.hpp"
#include "stats.hpp"

// C++ standard library
#include <iostream>
#include <chrono>
#include <cstring>
#include <queue>

Game::Game()
{
    gameState = GameState::MENU;
    flagsPlaced = 0;
    cellsRevealed = 0;
    clicks = 0;
    startTime = std::chrono::steady_clock::now();
}

Game::Game(GameState gameState)
    : gameState(gameState), flagsPlaced(0), cellsRevealed(0), clicks(0), startTime(std::chrono::steady_clock::now())
{
}

Game::Game(GameState gameState, Map gameMap)
    : gameMap(gameMap), gameState(gameState), flagsPlaced(0), cellsRevealed(0), clicks(0), startTime(std::chrono::steady_clock::now())
{
}

Game::Game(GameState gameState, Map gameMap, unsigned int flagsPlaced, unsigned int cellsRevealed, unsigned int clicks, std::chrono::steady_clock::time_point startTime)
    : gameMap(gameMap), gameState(gameState), flagsPlaced(flagsPlaced), cellsRevealed(cellsRevealed), clicks(clicks), startTime(startTime)
{
}

Game::Game(const Game &game)
    : gameMap(game.gameMap), gameState(game.gameState), flagsPlaced(game.flagsPlaced), cellsRevealed(game.cellsRevealed), clicks(game.clicks), startTime(game.startTime)
{
}

Game::~Game()
{
}

void Game::startGame()
{
    gameState = GameState::IN_PROGRESS;
    flagsPlaced = 0;
    cellsRevealed = 0;
    clicks = 0;
    startTime = std::chrono::steady_clock::now();
}

void Game::reset()
{
    gameMap.clearGrid();
    gameState = GameState::NOT_STARTED;
    flagsPlaced = 0;
    cellsRevealed = 0;
    clicks = 0;
}

void Game::setMap(const Map &map)
{
    this->gameMap = map;
}

void Game::revealCell(int x, int y)
{
    if (!gameMap.isValidPosition(x, y) || gameMap.isRevealedAt(x, y) || gameMap.isFlaggedAt(x, y))
        return;

    clicks++;
    gameMap.revealAt(x, y);
    cellsRevealed++;

    if (gameMap.isBombAt(x, y))
        loseGame();
    else
    {
        if (gameMap.getAdjacentBombsAt(x, y) == 0)
            fillReveal(x, y);

        checkWinCondition();
    }
}

void Game::fillReveal(int x, int y)
{
    /*
        Standard Minesweeper reveal algorithm using BFS with 8-connectivity.
    */

    std::queue<std::pair<int, int>> Queue;
    Queue.push(std::make_pair(x, y));

    while (!Queue.empty())
    {
        int i = Queue.front().first, j = Queue.front().second;
        Queue.pop();

        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                if (dx == 0 && dy == 0)
                    continue;

                int nx = i + dx, ny = j + dy;

                if (gameMap.isValidPosition(nx, ny) && !gameMap.isRevealedAt(nx, ny) && !gameMap.isFlaggedAt(nx, ny))
                {
                    if (!gameMap.isBombAt(nx, ny))
                    {
                        gameMap.revealAt(nx, ny);
                        cellsRevealed++;

                        if (gameMap.getAdjacentBombsAt(nx, ny) == 0)
                            Queue.push(std::make_pair(nx, ny));
                    }
                }
            }
        }
    }
}

void Game::adjacentReveal(int x, int y)
{
    // Functia implementeaza tehnica de Chording

    /*
       "When an uncovered square with a number has exactly the correct number of adjacent squares flagged,
    performing a click with both mouse buttons on it will uncover all unmarked squares;
        This is called a Chord."

        Sursa explicatie: https://www.speedrun.com/de-DE/gm/guides/0jd4m
    */
    if (!gameMap.isValidPosition(x, y) || !gameMap.isRevealedAt(x, y))
        return;

    clicks++;
    int flaggedNeighbors = 0;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            int nx = x + dx, ny = y + dy;
            if (gameMap.isValidPosition(nx, ny) && gameMap.isFlaggedAt(nx, ny))
                flaggedNeighbors++;
        }
    }

    if (flaggedNeighbors == gameMap.getAdjacentBombsAt(x, y))
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                int nx = x + dx, ny = y + dy;
                if (gameMap.isValidPosition(nx, ny) && !gameMap.isRevealedAt(nx, ny) && !gameMap.isFlaggedAt(nx, ny))
                {
                    clicks--;
                    revealCell(nx, ny);
                }
            }
        }
}

void Game::revealFullMap()
{
    gameMap.revealAll();
}


void Game::toggleFlag(int x, int y)
{
    if (!gameMap.isValidPosition(x, y) || gameMap.isRevealedAt(x, y))
        return;

    clicks++;
    gameMap.toggleFlagAt(x, y);

    if (gameMap.isFlaggedAt(x, y))
        flagsPlaced++;
    else
        flagsPlaced--;
}

Game::GameState Game::getCurrentState() const
{
    return gameState;
}

Map &Game::getMap()
{
    return gameMap;
}


const Map &Game::getMap() const
{
    return gameMap;
}

std::chrono::duration<int, std::milli> Game::getElapsedTime() const
{
    if (gameState == GameState::NOT_STARTED || gameState == GameState::MENU)
        return std::chrono::duration<int, std::milli>(0);

    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(now - startTime);
}

std::chrono::steady_clock::time_point Game::getStartTime() const
{
    return startTime;
}

unsigned int Game::getClicks() const
{
    return clicks;
}

unsigned int Game::getCellsRevealed() const
{
    return cellsRevealed;
}

unsigned int Game::getFlagsPlaced() const
{
    return flagsPlaced;
}

Game &Game::operator=(const Game &other)
{
    if (this == &other)
        return *this;

    gameMap = other.gameMap;
    gameState = other.gameState;
    flagsPlaced = other.flagsPlaced;
    cellsRevealed = other.cellsRevealed;
    clicks = other.clicks;
    startTime = other.startTime;

    return *this;
}

bool Game::operator==(const Game &other) const
{
    return gameMap == other.gameMap &&
           gameState == other.gameState &&
           flagsPlaced == other.flagsPlaced &&
           cellsRevealed == other.cellsRevealed &&
           clicks == other.clicks &&
           startTime == other.startTime;
}

bool Game::operator!=(const Game &other) const
{
    return !(*this == other);
}

bool Game::operator>=(const Game &other) const
{
    return !(*this < other);
}

bool Game::operator<=(const Game &other) const
{
    return !(*this > other);
}

bool Game::operator>(const Game &other) const
{
    return other < *this;
}

bool Game::operator<(const Game &other) const
{
    if (cellsRevealed != other.cellsRevealed)
        return cellsRevealed < other.cellsRevealed;
    return getElapsedTime() > other.getElapsedTime();
}

Game &Game::operator+(unsigned int bombs)
{
    gameMap = gameMap + bombs;
    return *this;
}

Game &Game::operator*(double factor)
{
    gameMap = gameMap * factor;
    return *this;
}

Game &Game::operator++()
{
    ++gameMap;
    return *this;
}

Cell &Game::operator[](int index)
{
    return gameMap[index];
}

void Game::checkWinCondition()
{
    for (unsigned int y = 0; y < gameMap.getHeight(); ++y)
        for (unsigned int x = 0; x < gameMap.getWidth(); ++x)
            if (!gameMap[y * gameMap.getWidth() + x]['b']    
                && !gameMap[y * gameMap.getWidth() + x]['r'])
            return;

    gameState = GameState::WON;
}

void Game::loseGame()
{
    gameState = GameState::LOST;
}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    os << "Flags placed: " << game.flagsPlaced << "\n";
    os << "Cells revealed: " << game.cellsRevealed << "\n";
    os << "Clicks: " << game.clicks << "\n";

    return os;
}

std::istream &operator>>(std::istream &is, Game &game)
{
    int state;
    if (is >> state >> game.flagsPlaced >> game.cellsRevealed >> game.clicks)
    {
        game.gameState = static_cast<Game::GameState>(state);
        is >> game.gameMap;
    }
    return is;
}
