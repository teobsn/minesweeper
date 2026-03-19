#pragma once

// Minesweeper headers
#include "cell.hpp"
#include "map.hpp"

// C++ standard library
#include <chrono>

class Match
{
private:
    unsigned int mapWidth, mapHeight;
    unsigned long long mapSeed;
    unsigned int mapBombs;

    unsigned int revealedCells;
    unsigned int clicks;
    bool finished = false;
    bool won = false;

    std::chrono::duration<int, std::milli> timePlayed;

public:
    Match();
    Match(const unsigned int revealedCells, const unsigned int mapBombs);
    Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned int mapBombs, const unsigned int mapSeed);
    Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs, const unsigned int revealedCells, const unsigned int clicks, const std::chrono::duration<int, std::milli> timePlayed, bool won, bool finished);
    ~Match();

    Match(const Match &other);

    unsigned int getMapWidth() const;
    unsigned int getMapHeight() const;
    unsigned int getMapSeed() const;
    unsigned int getMapBombs() const;
    void setMapInfo(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs);

    unsigned int getRevealedCells() const;
    void setRevealedCells(const unsigned int revealedCells);

    std::chrono::duration<int, std::milli> getTimePlayed() const;
    void setTimePlayed(std::chrono::duration<int, std::milli> timePlayed);

    unsigned int getClicks() const;
    void setClicks(const unsigned int clicks);
    void incrementClicks();

    bool getWinState() const;
    void setWinState(bool won);

    bool isFinished() const;
    void setFinished(bool finished);

    // Operator de atribuire
    Match &operator=(const Match &other);

    // Operatori de comparatie
    bool operator==(const Match &other) const;
    bool operator!=(const Match &other) const;

    bool operator>=(const Match &other) const;
    bool operator<=(const Match &other) const;
    bool operator>(const Match &other) const;
    bool operator<(const Match &other) const;

    // Operatori aritmetici
    Match &operator+(/**/);
    Match &operator*(/**/);
    Match &operator++();

    // Operator []
    Cell operator[](int index);

    // Friend pentru ostream, istream
    friend std::ostream &operator<<(std::ostream &os, const Match &match);
    friend std::istream &operator>>(std::istream &os, Match &match);
};

std::ostream &operator<<(std::ostream &os, const Match &match);
std::istream &operator>>(std::istream &os, Match &match);

class PlayerStats
{
private:
    char *playerName;

    Match *games;
    int numberOfGames;
    int numberOfWins;

    float winRate;

public:
    PlayerStats();
    ~PlayerStats();
    PlayerStats(const PlayerStats &stats);

    char *getPlayerName();
    const char *getPlayerName() const;
    void setPlayerName(const char *newName);

    void addMatch(Match match);
    Match *getMatches();
    const Match *getMatches() const;

    float getWinRate() const;
    void setWinRate(float winRate);
    void updateWinRate();

    unsigned int getWins() const;
    void updateWins();

    int getMatchesNumber() const;

    // Operatori de comparatie
    PlayerStats &operator=(const PlayerStats &other);
    bool operator==(const PlayerStats &other) const;
    bool operator!=(const PlayerStats &other) const;

    bool operator>=(const PlayerStats &other) const;
    bool operator<=(const PlayerStats &other) const;
    bool operator>(const PlayerStats &other) const;
    bool operator<(const PlayerStats &other) const;

    // Operatori aritmetici
    PlayerStats &operator+(/**/);
    PlayerStats &operator*(/**/);
    PlayerStats &operator++();

    // Operator []
    unsigned int operator[](char c);

    // Friend pentru ostream, istream
    friend std::ostream &operator<<(std::ostream &os, const PlayerStats &plrStats);
    friend std::istream &operator>>(std::istream &os, PlayerStats &plrStats);
};

std::ostream &operator<<(std::ostream &os, const PlayerStats &plrStats);
std::istream &operator>>(std::istream &os, PlayerStats &plrStats);

class Leaderboard
{
private:
    static PlayerStats *playerStats;
    static unsigned int players;
    static unsigned int overallGames;
    static unsigned int capacity;

    static void reallocate(unsigned int newCapacity);

public:
    Leaderboard();
    ~Leaderboard();

    static PlayerStats *findPlayer(const char *name);
    static PlayerStats *addPlayer(const char *name);
    static PlayerStats *getPlayer(unsigned int index);
    static unsigned int getPlayerCount();
    static unsigned int getOverallGames();
    static void addGame();
};