
// Minesweeper headers
#include "game.hpp"
#include "cell.hpp"
#include "map.hpp"
#include "stats.hpp"
#include "powerups.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

// C++ standard library
#include <iostream>
#include <chrono>
#include <cstring>
#include <algorithm>
#include <queue>
#include <typeinfo>
#include <sstream>

Game::Game()
{
#ifdef DEBUG
    std::cout << "Game()\n";
#endif

    gameState = GameState::MENU;
    flagsPlaced = 0;
    cellsRevealed = 0;
    clicks = 0;
    startTime = std::chrono::steady_clock::now();
}

Game::Game(GameState gameState)
    : gameState(gameState), flagsPlaced(0), cellsRevealed(0), clicks(0), startTime(std::chrono::steady_clock::now())
{
#ifdef DEBUG
    std::cout << "Game()\n";
#endif
}

Game::Game(GameState gameState, Map gameMap)
    : gameMap(gameMap), gameState(gameState), flagsPlaced(0), cellsRevealed(0), clicks(0), startTime(std::chrono::steady_clock::now())
{
#ifdef DEBUG
    std::cout << "Game()\n";
#endif
}

Game::Game(GameState gameState, Map gameMap, unsigned int flagsPlaced, unsigned int cellsRevealed, unsigned int clicks, std::chrono::steady_clock::time_point startTime)
    : gameMap(gameMap), gameState(gameState), flagsPlaced(flagsPlaced), cellsRevealed(cellsRevealed), clicks(clicks), startTime(startTime)
{
#ifdef DEBUG
    std::cout << "Game()\n";
#endif
}

Game::Game(const Game &game)
    : IObject(game), gameMap(game.gameMap), gameState(game.gameState), flagsPlaced(game.flagsPlaced), cellsRevealed(game.cellsRevealed), clicks(game.clicks), startTime(game.startTime), invulnerable(game.invulnerable)
{
    // Istoricul nu se copiaza pentru a evita double free
    this->powerUpHistory.clear();
}

Game::~Game()
{
#ifdef DEBUG
    std::cout << "~Game()\n";
#endif
    clearPowerUpHistory();
}

void Game::startGame()
{
    gameState = GameState::IN_PROGRESS;
    startTime = std::chrono::steady_clock::now();
    Logger::getInstance().info("Game started");
}

void Game::reset()
{
    gameMap.clearGrid();
    gameState = GameState::NOT_STARTED;
    flagsPlaced = 0;
    cellsRevealed = 0;
    clicks = 0;
    invulnerable = false;
    clearPowerUpHistory();
    Logger::getInstance().info("Game reset");
}

void Game::clearPowerUpHistory()
{
    // Eliberam explicit resursele cu for_each, apoi curatam lista prin remove_if
    // (e usor artificiala implementarea...)
    std::for_each(powerUpHistory.begin(), powerUpHistory.end(), [](BasePowerUp *&p)
                  {
        delete p;
        p = nullptr; });

    powerUpHistory.remove_if([](BasePowerUp *p)
                             { return p == nullptr; });
}

void Game::setMap(const Map &map)
{
    this->gameMap = map;
}

void Game::revealCell(int x, int y)
{
#ifdef DEBUG
    Logger::getInstance().debug("Reveal click at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
#endif
    if (!gameMap.isValidPosition(x, y))
        throw InvalidMoveException("Invalid move: position outside the map", x, y);

    if (gameMap.isRevealedAt(x, y) || gameMap.isFlaggedAt(x, y))
        throw InvalidMoveException("Invalid move: cell cannot be revealed", x, y);

    clicks++;

    if (gameMap.isBombAt(x, y))
    {
        if (invulnerable)
        {
            gameMap.revealAt(x, y);
            cellsRevealed++;
            invulnerable = false; // Consumare imediata
        }
        else
        {
            gameMap.revealAt(x, y);
            cellsRevealed++;
            loseGame();
            return;
        }
    }
    else
    {
        gameMap.revealAt(x, y);
        cellsRevealed++;
        if (gameMap.getAdjacentBombsAt(x, y) == 0)
            fillReveal(x, y);
    }

    checkWinCondition();
}

void Game::setInvulnerable(bool value)
{
    invulnerable = value;
    if (value)
        Logger::getInstance().info("Shield activated");
}

bool Game::isInvulnerable() const
{
    return invulnerable;
}

void Game::consumeShield()
{
    invulnerable = false;
    Logger::getInstance().info("Shield consumed");
}

void Game::activateRadar(int x, int y, int radius)
{
    if (gameState == GameState::NOT_STARTED)
        startGame();
    gameMap.revealArea(x, y, radius);
    checkWinCondition();
#ifdef DEBUG
    Logger::getInstance().debug("Radar used at (" + std::to_string(x) + ", " + std::to_string(y) + ") radius=" + std::to_string(radius));
#endif
    std::ostringstream oss;
    oss << "Radar activated at (" << x << ", " << y << ") with radius " << radius;
    Logger::getInstance().info(oss.str());
}

void Game::addPowerUpToHistory(BasePowerUp *p)
{
    if (p)
        powerUpHistory.push_back(p);
}

unsigned int Game::getPowerUpCount(const std::string &type) const
{
    if (type == "all")
        return powerUpHistory.size();

    return static_cast<unsigned int>(std::count_if(powerUpHistory.begin(), powerUpHistory.end(), [&](BasePowerUp *p)
                                                   {
        if (!p)
            return false;

        // Folosim dynamic_cast cu tratarea exceptiei std::bad_cast
        // Folosim cast pe referinte pentru a prinde std::bad_cast in caz de esec.
        // Este preferat fata de static_cast deoarece ne permite sa validam
        // in siguranta la runtime ce fel de obiect derivat avem in colectie
        // (powerUpHistory contine toate tipurile de power-upuri)
        try {
            if (type == "Shield") {
                (void)dynamic_cast<ShieldPowerUp&>(*p);
                return true;
            } else if (type == "Scanner") {
                (void)dynamic_cast<ScannerPowerUp&>(*p);
                return true;
            } else if (type == "Ultimate") {
                (void)dynamic_cast<UltimatePowerUp&>(*p);
                return true;
            } else if (type == "Defuser") {
                (void)dynamic_cast<BombDefuser&>(*p);
                return true;
            }
        } catch (const std::bad_cast&) {
            return false;
        }

        return false; }));
}

void Game::showPowerUpHistory() const
{
    std::for_each(powerUpHistory.begin(), powerUpHistory.end(), [](BasePowerUp *p)
                  {
        if (p) {
            std::cout << *p << "\n";
        } });
}

void Game::fillReveal(int x, int y)
{
    /*
       Standard Minesweeper reveal algorithm using BFS with 8-connectivity.
    */

    // std::queue ofera FIFO cu push/pop O(1) amortizat, ce ne este folositor
    // pentru BFS-ul de flood fill.
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
#ifdef DEBUG
    Logger::getInstance().debug("Chord at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
#endif
    // Functia implementeaza tehnica de Chording

    /*
       "When an uncovered square with a number has exactly the correct number of
       adjacent squares flagged, performing a click with both mouse buttons on it
       will uncover all unmarked squares. This is called a Chord."

       Sursa explicatie: https://www.speedrun.com/de-DE/gm/guides/0jd4m
    */
    if (!gameMap.isValidPosition(x, y) || !gameMap.isRevealedAt(x, y))
        throw InvalidMoveException("Invalid move: chord used on an unavailable cell", x, y);

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
#ifdef DEBUG
    Logger::getInstance().debug("Flag toggle at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
#endif
    if (!gameMap.isValidPosition(x, y) || gameMap.isRevealedAt(x, y))
        throw InvalidMoveException("Invalid move: cannot place a flag on this cell", x, y);

    clicks++;
    gameMap.toggleFlagAt(x, y);

    if (gameMap.isFlaggedAt(x, y))
        flagsPlaced++;
    else
        flagsPlaced--;
}

std::string Game::toString() const
{
    std::ostringstream os;
    os << "Game #" << getId() << " State: ";

    switch (gameState)
    {
    case GameState::MENU:
        os << "MENU";
        break;
    case GameState::NOT_STARTED:
        os << "NOT_STARTED";
        break;
    case GameState::IN_PROGRESS:
        os << "IN_PROGRESS";
        break;
    case GameState::WON:
        os << "WON";
        break;
    case GameState::LOST:
        os << "LOST";
        break;
    }

    os << " | Flags: " << flagsPlaced
       << " | Revealed: " << cellsRevealed
       << " | Clicks: " << clicks;
    return os.str();
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

    clearPowerUpHistory();

    gameMap = other.gameMap;
    gameState = other.gameState;
    flagsPlaced = other.flagsPlaced;
    cellsRevealed = other.cellsRevealed;
    clicks = other.clicks;
    startTime = other.startTime;
    invulnerable = other.invulnerable;

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
    checkWinCondition();
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
    if (gameState == GameState::WON || gameState == GameState::LOST)
        return;

    for (unsigned int y = 0; y < gameMap.getHeight(); ++y)
        for (unsigned int x = 0; x < gameMap.getWidth(); ++x)
            if (!gameMap[y * gameMap.getWidth() + x]['b'] && !gameMap[y * gameMap.getWidth() + x]['r'])
                return;

    gameState = GameState::WON;
    Logger::getInstance().info("Game won");
}

void Game::loseGame()
{
    if (gameState == GameState::LOST)
        return;

    gameState = GameState::LOST;
    Logger::getInstance().info("Game lost");
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
