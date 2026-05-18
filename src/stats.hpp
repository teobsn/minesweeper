#pragma once

// Minesweeper headers
#include "cell.hpp"
#include "iobject.hpp"
#include "map.hpp"
#include "templates.hpp"

// C++ standard library
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

class Match : public IObject
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

    unsigned int shieldsUsed = 0;
    unsigned int scannersUsed = 0;
    unsigned int ultimatesUsed = 0;
    unsigned int defusersUsed = 0;

public:
    Match();
    Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed);
    Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs);
    Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs, const unsigned int revealedCells, const unsigned int clicks, const std::chrono::duration<int, std::milli> timePlayed, bool won, bool finished);
    Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs, const unsigned int revealedCells, const unsigned int clicks, const std::chrono::duration<int, std::milli> timePlayed, bool won, bool finished, unsigned int shields, unsigned int scans, unsigned int ults, unsigned int defs);
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

    unsigned int getShieldsUsed() const;
    unsigned int getScannersUsed() const;
    unsigned int getUltimatesUsed() const;
    unsigned int getDefusersUsed() const;
    unsigned int getTotalPowerUpsUsed() const;

    void setPowerUpStats(unsigned int s, unsigned int sc, unsigned int u, unsigned int d);

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
    Match &operator+(int clicks);
    Match &operator-(int clicks);
    Match &operator++();

    // Friend pentru +, -
    friend Match &operator+(int value, Match &match);
    friend Match &operator-(int value, Match &match);

    // Operator []
    unsigned int operator[](char c) const;

    std::string toString() const override;

    // Friend pentru ostream, istream
    friend std::ostream &operator<<(std::ostream &os, const Match &match);
    friend std::istream &operator>>(std::istream &os, Match &match);
};

std::ostream &operator<<(std::ostream &os, const Match &match);
std::istream &operator>>(std::istream &os, Match &match);

class PlayerStats
{
private:
    std::string playerName;
    std::vector<Match> games;
    StaticQueue<Match, 10> recentGames;
    int numberOfWins;
    float winRate;

public:
    PlayerStats();
    explicit PlayerStats(const std::string &name);
    PlayerStats(const std::string &name, int numberOfGames, int numberOfWins);
    PlayerStats(const std::string &name, const std::vector<Match> &games, int numberOfWins);
    PlayerStats(const std::string &name, const std::vector<Match> &games, int numberOfWins, float winRate);
    ~PlayerStats() noexcept = default;
    PlayerStats(const PlayerStats &stats) = default;
    PlayerStats(PlayerStats &&stats) noexcept = default;

    std::string &getPlayerName() noexcept;
    const std::string &getPlayerName() const noexcept;
    void setPlayerName(const std::string &newName);

    void addMatch(const Match &match);
    void removeMatches(int numMatches);

    std::vector<Match> &getMatches() noexcept;
    const std::vector<Match> &getMatches() const noexcept;
    const StaticQueue<Match, 10> &getRecentMatches() const noexcept;

    float getWinRate() const;
    void setWinRate(float winRate);
    void updateWinRate();

    unsigned int getWins() const;
    void updateWins();

    int getMatchesNumber() const;

    // Operatori de comparatie
    PlayerStats &operator=(const PlayerStats &other);
    PlayerStats &operator=(PlayerStats &&other) noexcept = default;
    bool operator==(const PlayerStats &other) const;
    bool operator!=(const PlayerStats &other) const;

    bool operator>=(const PlayerStats &other) const;
    bool operator<=(const PlayerStats &other) const;
    bool operator>(const PlayerStats &other) const;
    bool operator<(const PlayerStats &other) const;

    // Operatori aritmetici
    PlayerStats &operator+(Match game);
    PlayerStats &operator-(int numMatches);
    PlayerStats &operator--();

    // Operator []
    unsigned int operator[](char c) const;

    // Friend pentru ostream, istream
    friend std::ostream &operator<<(std::ostream &os, const PlayerStats &plrStats);
    friend std::istream &operator>>(std::istream &os, PlayerStats &plrStats);
};

std::ostream &operator<<(std::ostream &os, const PlayerStats &plrStats);
std::istream &operator>>(std::istream &os, PlayerStats &plrStats);

class Leaderboard
{
private:
    static std::unordered_map<std::string, PlayerStats> playerStats;
    static std::vector<std::string> playerOrder;
    static unsigned int overallGames;

public:
    Leaderboard();
    ~Leaderboard();

    static PlayerStats *findPlayer(const std::string &name);
    static PlayerStats *addPlayer(const std::string &name);
    static PlayerStats *getPlayer(unsigned int index);
    static std::vector<PlayerStats> getPlayers();
    static unsigned int getPlayerCount();
    static unsigned int getOverallGames();
    static void addGame();
    static void removePlayer(const std::string &name);
    static void sort(char order = 'd', bool equality = false);
};

template<>
const PlayerStats *findBest<PlayerStats>(const std::vector<PlayerStats> &values);
