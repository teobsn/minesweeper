// Minesweeper headers
#include "stats.hpp"
#include "cell.hpp"
#include "map.hpp"

// C++ standard library
#include <iostream>
#include <chrono>
#include <cstring>
#include <algorithm>

Match::Match() : mapWidth(0), mapHeight(0), mapSeed(0), mapBombs(0),
                 revealedCells(0), clicks(0), finished(false), won(false), timePlayed(0) {}

Match::Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed)
    : mapWidth(mapWidth), mapHeight(mapHeight), mapSeed(mapSeed), mapBombs(0),
      revealedCells(0), clicks(0), finished(false), won(false), timePlayed(0)
{
}

Match::Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs)
    : mapWidth(mapWidth), mapHeight(mapHeight), mapSeed(mapSeed), mapBombs(mapBombs),
      revealedCells(0), clicks(0), finished(false), won(false), timePlayed(0)
{
}

Match::Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs, const unsigned int revealedCells, const unsigned int clicks, const std::chrono::duration<int, std::milli> timePlayed, bool won, bool finished)
    : mapWidth(mapWidth), mapHeight(mapHeight), mapSeed(mapSeed), mapBombs(mapBombs), revealedCells(revealedCells), clicks(clicks), finished(finished), won(won), timePlayed(timePlayed)
{
}

Match::~Match()
{
    std::cout << "\n~Match()\n";
}

Match::Match(const Match &other)
    : mapWidth(other.mapWidth), mapHeight(other.mapHeight), mapSeed(other.mapSeed), mapBombs(other.mapBombs)
{
    this->revealedCells = other.revealedCells;
    this->clicks = other.clicks;
    this->finished = other.finished;
    this->won = other.won;
    this->timePlayed = other.timePlayed;
}

unsigned int Match::getMapWidth() const
{
    return mapWidth;
}

unsigned int Match::getMapHeight() const
{
    return mapHeight;
}

unsigned int Match::getMapSeed() const
{
    return mapSeed;
}

unsigned int Match::getMapBombs() const
{
    return mapBombs;
}

void Match::setMapInfo(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs)
{
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    this->mapSeed = mapSeed;
    this->mapBombs = mapBombs;
}

unsigned int Match::getRevealedCells() const
{
    return revealedCells;
}

void Match::setRevealedCells(const unsigned int revealedCells)
{
    this->revealedCells = revealedCells;
}

std::chrono::duration<int, std::milli> Match::getTimePlayed() const
{
    return timePlayed;
}
void Match::setTimePlayed(std::chrono::duration<int, std::milli> timePlayed)
{
    this->timePlayed = timePlayed;
}

unsigned int Match::getClicks() const
{
    // return clicks;
    return (*this)['c'];
}

void Match::setClicks(const unsigned int clicks)
{
    this->clicks = clicks;
}

void Match::incrementClicks()
{
    // this->clicks++;
    ++(*this);
}

bool Match::getWinState() const
{
    // return won;
    return (*this)['w'];
}

void Match::setWinState(bool won)
{
    this->won = won;
}

bool Match::isFinished() const
{
    // return finished;
    return (*this)['f'];
}

void Match::setFinished(bool finished)
{
    this->finished = finished;
}

Match &Match::operator=(const Match &other)
{
    if (this == &other)
        return *this;

    mapWidth = other.mapWidth;
    mapHeight = other.mapHeight;
    mapSeed = other.mapSeed;
    mapBombs = other.mapBombs;
    revealedCells = other.revealedCells;
    clicks = other.clicks;
    finished = other.finished;
    won = other.won;
    timePlayed = other.timePlayed;

    return *this;
}

bool Match::operator==(const Match &other) const
{
    return mapWidth == other.mapWidth && mapHeight == other.mapHeight &&
           mapSeed == other.mapSeed && mapBombs == other.mapBombs &&
           revealedCells == other.revealedCells && clicks == other.clicks &&
           finished == other.finished && won == other.won &&
           timePlayed == other.timePlayed;
}

bool Match::operator!=(const Match &other) const
{
    return !(*this == other);
}

bool Match::operator<(const Match &other) const
{
    if (won != other.won)
        return other.won;
    if (revealedCells != other.revealedCells)
        return revealedCells < other.revealedCells;
    return timePlayed > other.timePlayed;
}

bool Match::operator>(const Match &other) const
{
    return other < *this;
}

bool Match::operator<=(const Match &other) const
{
    return !(*this > other);
}

bool Match::operator>=(const Match &other) const
{
    return !(*this < other);
}

Match &Match::operator+(int clicks)
{
    this->clicks += clicks;
    return *this;
}

Match &Match::operator-(int clicks)
{
    this->clicks -= clicks;
    return *this;
}

Match &operator+(int value, Match &match)
{
    match.clicks += value;
    return match;
}

Match &operator-(int value, Match &match)
{
    match.clicks -= value;
    return match;
}

Match &Match::operator++()
{
    ++clicks;
    return *this;
}

unsigned int Match::operator[](char c) const
{
    switch (c)
    {
    case 'w':
    case 0:
        return won;
        break;
    case 'r':
    case 1:
        return revealedCells;
        break;
    case 't':
    case 2:
        return timePlayed.count();
        break;
    case 'f':
    case 3:
        return finished;
        break;
    case 'c':
    case 4:
        return clicks;
        break;
    default:
        return 0;
    }
}

std::ostream &operator<<(std::ostream &os, const Match &match)
{
    os << "Match Details:\n"
       << "  Map: " << match.mapWidth << "x" << match.mapHeight << "\n"
       << "  Seed: " << match.mapSeed << " | Bombs: " << match.mapBombs << "\n"
       << "  Status: " << (match.finished ? (match.won ? "Won" : "Lost") : "In Progress") << "\n"
       << "  Cells Revealed: " << match.revealedCells << "\n"
       << "  Clicks: " << match.clicks << "\n"
       << "  Time Played: " << (match.timePlayed.count() / 1000.0) << "s\n";

    return os;
}

std::istream &operator>>(std::istream &is, Match &match)
{
    unsigned int width, height, bombs, revealed, clicks;
    unsigned long long seed;
    bool finished, won;
    int timePlayedMs;

    if (is >> width >> height >> seed >> bombs >> revealed >> clicks >> finished >> won >> timePlayedMs)
    {
        match.setMapInfo(width, height, seed, bombs);
        match.setRevealedCells(revealed);
        match.setClicks(clicks);
        match.finished = finished;
        match.won = won;
        match.timePlayed = std::chrono::duration<int, std::milli>(timePlayedMs);
    }

    return is;
}

PlayerStats::PlayerStats() : playerName(nullptr), games(nullptr), numberOfGames(0), numberOfWins(0), winRate(0.0f)
{
}

PlayerStats::PlayerStats(const char *name)
    : games(nullptr), numberOfGames(0), numberOfWins(0), winRate(0.0f)
{
    playerName = new char[strlen(name) + 1];
    strcpy(playerName, name);
}

PlayerStats::PlayerStats(const char *name, int numberOfGames, int numberOfWins)
    : games(nullptr), numberOfGames(numberOfGames), numberOfWins(numberOfWins), winRate(0.0f)
{
    playerName = new char[strlen(name) + 1];
    strcpy(playerName, name);
}

PlayerStats::PlayerStats(const char *name, Match *games, int numberOfGames, int numberOfWins)
     : numberOfGames(numberOfGames), numberOfWins(numberOfWins), winRate(0.0f)
{
    playerName = new char[strlen(name) + 1];
    strcpy(playerName, name);

    // Presupunem ca games va tine strict de instanta curenta
    this->games = games;

    updateWinRate();
}

PlayerStats::PlayerStats(const char *name, Match *games, int numberOfGames, int numberOfWins, float winRate)
    : numberOfGames(numberOfGames), numberOfWins(numberOfWins), winRate(winRate)
{
    playerName = new char[strlen(name) + 1];
    strcpy(playerName, name);

    // Presupunem ca games va tine strict de instanta curenta
    this->games = games;

    //updateWinRate() // e deja dat de lista de initializare
}

PlayerStats::PlayerStats(const PlayerStats &stats) : playerName(nullptr), numberOfGames(stats.numberOfGames), numberOfWins(stats.numberOfWins), winRate(stats.winRate)
{
    if (stats.playerName)
    {
        playerName = new char[strlen(stats.playerName) + 1];
        strcpy(playerName, stats.playerName);
    }

    if (numberOfGames > 0 && stats.games != nullptr)
    {
        games = new Match[numberOfGames];
        for (int i = 0; i < numberOfGames; ++i)
        {
            games[i] = stats.games[i];
        }
    }
    else
        games = nullptr;
}

PlayerStats::~PlayerStats()
{
    if (games != nullptr)
        delete[] games;
    
    if (playerName != nullptr)
        delete[] playerName;

    std::cout << "\n~PlayerStats()\n";
}

char *PlayerStats::getPlayerName()
{
    return playerName;
}

const char *PlayerStats::getPlayerName() const
{
    return playerName;
}

void PlayerStats::setPlayerName(const char *newName)
{
    if (playerName)
        delete[] playerName;
    if (newName)
    {
        playerName = new char[strlen(newName) + 1];
        strcpy(playerName, newName);
    }
    else
    {
        playerName = nullptr;
    }
}

void PlayerStats::addMatch(Match match)
{
    // Alocam vectorul nou
    Match *newGames = new Match[numberOfGames + 1];

    // Copiem meciurile vechi
    for (int i = 0; i < numberOfGames; ++i)
        newGames[i] = games[i];

    // Adaugam meciul nou
    newGames[numberOfGames] = match;

    // Dezalocam (asa e in limba romana?) vectorul vechi
    delete[] games;

    // Modificam pointerul la vectorul nou
    games = newGames;

    // Incrementam numarul de jocuri
    numberOfGames++;

    // Calculam numarul de castiguri
    int wins = 0;
    for (int i = 0; i < numberOfGames; ++i)
        if (games[i].getWinState())
            wins++;

    this->numberOfWins = wins;

    winRate = numberOfGames > 0 ? (float)wins / numberOfGames : 0.0f;
}

void PlayerStats::removeMatches(int numMatches)
{
    if (numMatches <= 0 || numMatches > numberOfGames)
        return;

    Match *newGames = new Match[numberOfGames - numMatches];
    for (int j = 0; j < numberOfGames - numMatches; ++j)
        newGames[j] = games[j];
    delete[] games;
    games = newGames;

    numberOfGames -= numMatches;

    updateWins();
    updateWinRate();
}

Match *PlayerStats::getMatches()
{
    return games;
}

const Match *PlayerStats::getMatches() const
{
    return games;
}

float PlayerStats::getWinRate() const
{
    return winRate;
}

void PlayerStats::setWinRate(float winRate)
{
    this->winRate = winRate;
}

void PlayerStats::updateWinRate()
{
    int wins = 0;
    for (int i = 0; i < numberOfGames; ++i)
        if (games[i].getWinState())
            wins++;

    setWinRate((float)wins / numberOfGames);
}

unsigned int PlayerStats::getWins() const
{
    // return numberOfWins;
    return (*this)['w'];
}

void PlayerStats::updateWins()
{
    int wins = 0;
    for (int i = 0; i < numberOfGames; ++i)
        if (games[i].getWinState())
            wins++;

    numberOfWins = wins;
}

int PlayerStats::getMatchesNumber() const
{
    // return numberOfGames;
    return (*this)['n'];
}

PlayerStats &PlayerStats::operator=(const PlayerStats &other)
{
    if (this == &other)
        return *this;

    setPlayerName(other.playerName);

    delete[] games;

    numberOfGames = other.numberOfGames;
    winRate = other.winRate;

    if (numberOfGames > 0 && other.games != nullptr)
    {
        games = new Match[numberOfGames];
        for (int i = 0; i < numberOfGames; ++i)
            games[i] = other.games[i];
    }
    else
        games = nullptr;

    return *this;
}

bool PlayerStats::operator==(const PlayerStats &other) const
{
    if (playerName && other.playerName)
    {
        if (strcmp(playerName, other.playerName) != 0)
            return false;
    }
    else if (playerName != other.playerName)
        return false;

    if (numberOfGames != other.numberOfGames || winRate != other.winRate)
        return false;

    for (int i = 0; i < numberOfGames; ++i)
        if (games[i] != other.games[i])
            return false;

    return true;
}

bool PlayerStats::operator!=(const PlayerStats &other) const
{
    return !(*this == other);
}

bool PlayerStats::operator<(const PlayerStats &other) const
{
    return winRate < other.winRate;
}

bool PlayerStats::operator>(const PlayerStats &other) const
{
    return other < *this;
}

bool PlayerStats::operator<=(const PlayerStats &other) const
{
    return !(*this > other);
}

bool PlayerStats::operator>=(const PlayerStats &other) const
{
    return !(*this < other);
}

PlayerStats &PlayerStats::operator+(Match game)
{
    addMatch(game);
    return *this;
}

PlayerStats &PlayerStats::operator-(int numMatches)
{
    removeMatches(numMatches);
    return *this;
}

PlayerStats &PlayerStats::operator--()
{
    this->removeMatches(1);
    return *this;
}

unsigned int PlayerStats::operator[](char c) const
{
    switch (c)
    {
    case 'n':
    case 0:
        return numberOfGames;
        break;
    case 'w':
    case 1:
        return numberOfWins;
        break;
    case 'r':
    case 2:
        return winRate;
        break;
    default:
        return 0;
    }
}

std::ostream &operator<<(std::ostream &os, const PlayerStats &plrStats)
{
    os << "Statistics for " << (plrStats.playerName ? plrStats.playerName : "Unknown Player") << "\n";

    os << "Games played: " << plrStats.numberOfGames << "\n";
    os << "Win Rate: " << plrStats.winRate << "\n";

    return os;
}

std::istream &operator>>(std::istream &is, PlayerStats &plrStats)
{
    char name[64];
    int numGames;
    float wRate;

    if (is >> name >> numGames >> wRate)
    {
        plrStats.setPlayerName(name);

        delete[] plrStats.games;
        plrStats.numberOfGames = numGames;
        plrStats.winRate = wRate;
        if (numGames > 0)
        {
            plrStats.games = new Match[numGames];
            for (int i = 0; i < numGames; ++i)
                is >> plrStats.games[i];
        }
        else
            plrStats.games = nullptr;
    }
    return is;
}

PlayerStats *Leaderboard::playerStats = nullptr;
unsigned int Leaderboard::players = 0;
unsigned int Leaderboard::overallGames = 0;
unsigned int Leaderboard::capacity = 0;

Leaderboard::Leaderboard()
{
}

Leaderboard::~Leaderboard()
{
    std::cout << "\n~Leaderboard()\n";

    delete[] playerStats;
}

void Leaderboard::reallocate(unsigned int newCapacity)
{
    PlayerStats *newStats = new PlayerStats[newCapacity];

    for (unsigned int i = 0; i < players; ++i)
        newStats[i] = playerStats[i];

    delete[] playerStats;
    playerStats = newStats;
    capacity = newCapacity;
}

PlayerStats *Leaderboard::findPlayer(const char *name)
{
    if (!name || players == 0)
        return nullptr;

    for (unsigned int i = 0; i < players; ++i)
    {
        const char *playerName = playerStats[i].getPlayerName();
        if (playerName && strcmp(playerName, name) == 0)
        {
            return &playerStats[i];
        }
    }
    return nullptr;
}

PlayerStats *Leaderboard::addPlayer(const char *name)
{
    PlayerStats *existing = findPlayer(name);
    if (existing)
        return existing;

    if (players >= capacity)
    {
        // Comportament similar cu cel din vectorul STL
        unsigned int newCapacity = capacity == 0 ? 4 : capacity * 2;
        reallocate(newCapacity);
    }

    playerStats[players].setPlayerName(name);
    ++players;

    return &playerStats[players - 1];
}

PlayerStats *Leaderboard::getPlayer(unsigned int index)
{
    if (index >= players)
        return nullptr;

    return &playerStats[index];
}

unsigned int Leaderboard::getPlayerCount()
{
    return players;
}

unsigned int Leaderboard::getOverallGames()
{
    return overallGames;
}

void Leaderboard::addGame()
{
    ++overallGames;
}