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

Match::Match(const unsigned int revealedCells, const unsigned int mapBombs) : Match()
{
    this->revealedCells = revealedCells;
    this->mapBombs = mapBombs;
}

Match::Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned int mapBombs, const unsigned int mapSeed) : Match()
{
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    this->mapBombs = mapBombs;
    this->mapSeed = mapSeed;
}

Match::Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs, const unsigned int revealedCells, const std::chrono::duration<int, std::milli> timePlayed, bool won, bool finished)
    : mapWidth(mapWidth), mapHeight(mapHeight), mapSeed(mapSeed), mapBombs(mapBombs), revealedCells(revealedCells), finished(finished), won(won), timePlayed(timePlayed)
{
}

Match::~Match()
{
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
    return clicks;
}

void Match::setClicks(const unsigned int clicks)
{
    this->clicks = clicks;
}

void Match::incrementClicks()
{
    this->clicks++;
}

bool Match::getWinState() const
{
    return won;
}

void Match::setWinState(bool won)
{
    this->won = won;
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

Match &Match::operator+()
{
    return *this;
}

Match &Match::operator*()
{
    return *this;
}

Match &Match::operator++()
{
    ++clicks;
    return *this;
}

Cell Match::operator[](int index) { return Cell(); }

std::ostream &operator<<(std::ostream &os, const Match &match)
{
    // os << match.mapWidth << " " << match.mapHeight << " " << match.mapSeed << " " << match.mapBombs << "\n"
    //    << match.revealedCells << " " << match.clicks << " " << match.finished << " " << match.won << "\n"
    //    << match.timePlayed.count();
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

PlayerStats::PlayerStats() : playerName(nullptr), games(nullptr), numberOfGames(0), winRate(0.0f)
{
}

PlayerStats::~PlayerStats()
{
    delete[] games;
    delete[] playerName;
}

PlayerStats::PlayerStats(const PlayerStats &stats) : playerName(nullptr), numberOfGames(stats.numberOfGames), winRate(stats.winRate)
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

char *PlayerStats::getPlayerName()
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

    // Copiem meciul vechi
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

    winRate = numberOfGames > 0 ? (float)wins / numberOfGames : 0.0f;
}

Match *PlayerStats::getMatches()
{
    return games;
}

float PlayerStats::getWinRate() const
{
    return winRate;
}

int PlayerStats::getMatchesNumber() const
{
    return numberOfGames;
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

PlayerStats &PlayerStats::operator+()
{
    return *this;
}

PlayerStats &PlayerStats::operator*()
{
    return *this;
}

PlayerStats &PlayerStats::operator++()
{
    return *this;
}

Cell PlayerStats::operator[](int index)
{
    return Cell();
}

std::ostream &operator<<(std::ostream &os, const PlayerStats &plrStats)
{
    os << "Statistics for " << (plrStats.playerName ? plrStats.playerName : "Unknown") << "\n";

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

Leaderboard::Leaderboard()
{
}
Leaderboard::~Leaderboard()
{
}

char *Leaderboard::getBestPlayer()
{
    return nullptr;
    // de impl
}

char *Leaderboard::getPlayers()
{
    return nullptr;
    // de impl
}

void Leaderboard::addPlayer()
{
    // de impl
}

void Leaderboard::removePlayer()
{
    // de impl
}