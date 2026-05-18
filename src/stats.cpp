// Minesweeper headers
#include "stats.hpp"
#include "cell.hpp"
#include "map.hpp"
#include "exceptions.hpp"

// C++ standard library
#include <iostream>
#include <chrono>
#include <algorithm>
#include <sstream>

Match::Match() : mapWidth(0), mapHeight(0), mapSeed(0), mapBombs(0),
                 revealedCells(0), clicks(0), finished(false), won(false), timePlayed(0),
                 shieldsUsed(0), scannersUsed(0), ultimatesUsed(0), defusersUsed(0) {}

Match::Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed)
    : mapWidth(mapWidth), mapHeight(mapHeight), mapSeed(mapSeed), mapBombs(0),
      revealedCells(0), clicks(0), finished(false), won(false), timePlayed(0),
      shieldsUsed(0), scannersUsed(0), ultimatesUsed(0), defusersUsed(0)
{
}

Match::Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs)
    : mapWidth(mapWidth), mapHeight(mapHeight), mapSeed(mapSeed), mapBombs(mapBombs),
      revealedCells(0), clicks(0), finished(false), won(false), timePlayed(0),
      shieldsUsed(0), scannersUsed(0), ultimatesUsed(0), defusersUsed(0)
{
}

Match::Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs, const unsigned int revealedCells, const unsigned int clicks, const std::chrono::duration<int, std::milli> timePlayed, bool won, bool finished)
    : mapWidth(mapWidth), mapHeight(mapHeight), mapSeed(mapSeed), mapBombs(mapBombs), revealedCells(revealedCells), clicks(clicks), finished(finished), won(won), timePlayed(timePlayed),
      shieldsUsed(0), scannersUsed(0), ultimatesUsed(0), defusersUsed(0)
{
}

Match::Match(const unsigned int mapWidth, const unsigned int mapHeight, const unsigned long long mapSeed, const unsigned int mapBombs, const unsigned int revealedCells, const unsigned int clicks, const std::chrono::duration<int, std::milli> timePlayed, bool won, bool finished, unsigned int shields, unsigned int scans, unsigned int ults, unsigned int defs)
    : mapWidth(mapWidth), mapHeight(mapHeight), mapSeed(mapSeed), mapBombs(mapBombs), revealedCells(revealedCells), clicks(clicks), finished(finished), won(won), timePlayed(timePlayed),
      shieldsUsed(shields), scannersUsed(scans), ultimatesUsed(ults), defusersUsed(defs)
{
}

Match::~Match()
{
#ifdef DEBUG
    std::cout << "\n~Match()\n";
#endif
}

Match::Match(const Match &other)
    : IObject(other), mapWidth(other.mapWidth), mapHeight(other.mapHeight), mapSeed(other.mapSeed), mapBombs(other.mapBombs)
{
    this->revealedCells = other.revealedCells;
    this->clicks = other.clicks;
    this->finished = other.finished;
    this->won = other.won;
    this->timePlayed = other.timePlayed;
    this->shieldsUsed = other.shieldsUsed;
    this->scannersUsed = other.scannersUsed;
    this->ultimatesUsed = other.ultimatesUsed;
    this->defusersUsed = other.defusersUsed;
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

unsigned int Match::getShieldsUsed() const
{
    return shieldsUsed;
}

unsigned int Match::getScannersUsed() const
{
    return scannersUsed;
}

unsigned int Match::getUltimatesUsed() const
{
    return ultimatesUsed;
}

unsigned int Match::getDefusersUsed() const
{
    return defusersUsed;
}

unsigned int Match::getTotalPowerUpsUsed() const
{
    return shieldsUsed + scannersUsed + ultimatesUsed + defusersUsed;
}

void Match::setPowerUpStats(unsigned int s, unsigned int sc, unsigned int u, unsigned int d)
{
    shieldsUsed = s;
    scannersUsed = sc;
    ultimatesUsed = u;
    defusersUsed = d;
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
    shieldsUsed = other.shieldsUsed;
    scannersUsed = other.scannersUsed;
    ultimatesUsed = other.ultimatesUsed;
    defusersUsed = other.defusersUsed;

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
    case 's':
    case 5:
        return shieldsUsed;
        break;
    case 'a':
    case 6:
        return scannersUsed;
        break;
    case 'u':
    case 7:
        return ultimatesUsed;
        break;
    case 'd':
    case 8:
        return defusersUsed;
        break;
    default:
        return 0;
    }
}

std::ostream &operator<<(std::ostream &os, const Match &match)
{
    return os << match.toString();
}

std::string Match::toString() const
{
    std::ostringstream os;
    os << "Match #" << getId() << "\n"
       << "  Map: " << mapWidth << "x" << mapHeight << "\n"
       << "  Seed: " << mapSeed << " | Bombs: " << mapBombs << "\n"
       << "  Status: " << (finished ? (won ? "Won" : "Lost") : "In Progress") << "\n"
       << "  Cells Revealed: " << revealedCells << "\n"
       << "  Clicks: " << clicks << "\n"
       << "  Time Played: " << (timePlayed.count() / 1000.0) << "s\n"
       << "  PowerUps Used: " << getTotalPowerUpsUsed() << " (S:" << shieldsUsed << ", A:" << scannersUsed << ", U:" << ultimatesUsed << ", D:" << defusersUsed << ")";
    return os.str();
}

std::istream &operator>>(std::istream &is, Match &match)
{
    unsigned int width, height, bombs, revealed, clicks;
    unsigned long long seed;
    bool finished, won;
    int timePlayedMs;
    unsigned int s, a, u, d;

    if (is >> width >> height >> seed >> bombs >> revealed >> clicks >> finished >> won >> timePlayedMs >> s >> a >> u >> d)
    {
        match.setMapInfo(width, height, seed, bombs);
        match.setRevealedCells(revealed);
        match.setClicks(clicks);
        match.setFinished(finished);
        match.setWinState(won);
        match.setTimePlayed(std::chrono::duration<int, std::milli>(timePlayedMs));
        match.setPowerUpStats(s, a, u, d);
    }

    return is;
}

PlayerStats::PlayerStats() : playerName(), games(), recentGames(), numberOfWins(0), winRate(0.0f)
{
}

PlayerStats::PlayerStats(const std::string &name)
    : playerName(name), games(), recentGames(), numberOfWins(0), winRate(0.0f)
{
}

PlayerStats::PlayerStats(const std::string &name, int numberOfGames, int numberOfWins)
    : playerName(name), games(), recentGames(), numberOfWins(numberOfWins), winRate(0.0f)
{
    games.reserve(numberOfGames);
}

PlayerStats::PlayerStats(const std::string &name, const std::vector<Match> &games, int numberOfWins)
    : playerName(name), games(games), recentGames(), numberOfWins(numberOfWins), winRate(0.0f)
{
    updateWinRate();
    updateWins();
}

PlayerStats::PlayerStats(const std::string &name, const std::vector<Match> &games, int numberOfWins, float winRate)
    : playerName(name), games(games), recentGames(), numberOfWins(numberOfWins), winRate(winRate)
{
    for (const Match &match : this->games)
    {
        if (recentGames.full())
            recentGames.pop();
        recentGames.push(match);
    }
}

std::string &PlayerStats::getPlayerName() noexcept
{
    return playerName;
}

const std::string &PlayerStats::getPlayerName() const noexcept
{
    return playerName;
}

void PlayerStats::setPlayerName(const std::string &newName)
{
    if (newName.empty())
        throw MinesweeperException("Player name cannot be empty");

    playerName = newName;
}

void PlayerStats::addMatch(const Match &match)
{
    games.push_back(match);

    if (recentGames.full())
        recentGames.pop();
    recentGames.push(match);

    updateWins();
    updateWinRate();
}

void PlayerStats::removeMatches(int numMatches)
{
    if (numMatches <= 0 || numMatches > getMatchesNumber())
        throw DatabaseException("Numar invalid de meciuri de eliminat", numMatches);

    games.erase(games.end() - numMatches, games.end());

    recentGames.clear();
    std::size_t start = games.size() > 10 ? games.size() - 10 : 0;
    for (std::size_t i = start; i < games.size(); ++i)
        recentGames.push(games[i]);

    updateWins();
    updateWinRate();
}

std::vector<Match> &PlayerStats::getMatches() noexcept
{
    return games;
}

const std::vector<Match> &PlayerStats::getMatches() const noexcept
{
    return games;
}

const StaticQueue<Match, 10> &PlayerStats::getRecentMatches() const noexcept
{
    return recentGames;
}

float PlayerStats::getWinRate() const
{
    return winRate;
}

void PlayerStats::setWinRate(float winRate)
{
    this->winRate = clampValue(winRate, 0.0f, 1.0f);
}

void PlayerStats::updateWinRate()
{
    if (games.empty())
    {
        setWinRate(0.0f);
        return;
    }

    unsigned int wins = 0;
    std::for_each(games.begin(), games.end(), [&](const Match &match)
    {
        if (match.getWinState())
            ++wins;
    });

    setWinRate(static_cast<float>(wins) / static_cast<float>(games.size()));
}

unsigned int PlayerStats::getWins() const
{
    return (*this)['w'];
}

void PlayerStats::updateWins()
{
    numberOfWins = static_cast<int>(std::count_if(games.begin(), games.end(), [](const Match &match)
    {
        return match.getWinState();
    }));
}

int PlayerStats::getMatchesNumber() const
{
    return (*this)['n'];
}

PlayerStats &PlayerStats::operator=(const PlayerStats &other)
{
    if (this == &other)
        return *this;

    playerName = other.playerName;
    games = other.games;
    recentGames = other.recentGames;
    numberOfWins = other.numberOfWins;
    winRate = other.winRate;

    return *this;
}

bool PlayerStats::operator==(const PlayerStats &other) const
{
    return playerName == other.playerName &&
           games == other.games &&
           numberOfWins == other.numberOfWins &&
           winRate == other.winRate;
}

bool PlayerStats::operator!=(const PlayerStats &other) const
{
    return !(*this == other);
}

bool PlayerStats::operator<(const PlayerStats &other) const
{
    if (winRate != other.winRate)
        return winRate < other.winRate;

    if (numberOfWins != other.numberOfWins)
        return numberOfWins < other.numberOfWins;

    return playerName < other.playerName;
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
        return static_cast<unsigned int>(games.size());
    case 'w':
    case 1:
        return static_cast<unsigned int>(numberOfWins);
    case 'r':
    case 2:
        return static_cast<unsigned int>(winRate * 100.0f);
    default:
        return 0;
    }
}

std::ostream &operator<<(std::ostream &os, const PlayerStats &plrStats)
{
    os << "Statistics for " << (plrStats.playerName.empty() ? "Unknown Player" : plrStats.playerName) << "\n";
    os << "Games played: " << plrStats.getMatchesNumber() << "\n";
    os << "Win Rate: " << plrStats.winRate << "\n";
    return os;
}

std::istream &operator>>(std::istream &is, PlayerStats &plrStats)
{
    std::string name;
    int numGames;
    float wRate;

    if (is >> name >> numGames >> wRate)
    {
        plrStats.setPlayerName(name);
        plrStats.games.clear();
        plrStats.recentGames.clear();
        plrStats.numberOfWins = 0;
        plrStats.setWinRate(wRate);

        if (numGames > 0)
        {
            plrStats.games.reserve(numGames);
            for (int i = 0; i < numGames; ++i)
            {
                Match match;
                is >> match;
                plrStats.addMatch(match);
            }
        }
    }
    return is;
}

std::unordered_map<std::string, PlayerStats> Leaderboard::playerStats = {};
std::vector<std::string> Leaderboard::playerOrder = {};
unsigned int Leaderboard::overallGames = 0;

Leaderboard::Leaderboard()
{
}

Leaderboard::~Leaderboard()
{
    std::cout << "\n~Leaderboard()\n";
}

PlayerStats *Leaderboard::findPlayer(const std::string &name)
{
    if (name.empty())
        throw MinesweeperException("Player name cannot be empty");

    auto it = playerStats.find(name);

    if (it == playerStats.end())
        return nullptr;

    return &it->second;
}

PlayerStats *Leaderboard::addPlayer(const std::string &name)
{
    if (name.empty())
        throw MinesweeperException("Player name cannot be empty");

    PlayerStats *existing = findPlayer(name);
    if (existing)
        return existing;

    auto [it, inserted] = playerStats.emplace(name, PlayerStats(name));
    if (inserted)
        playerOrder.push_back(name);

    return &it->second;
}

PlayerStats *Leaderboard::getPlayer(unsigned int index)
{
    if (index >= playerOrder.size())
        throw DatabaseException("Invalid leaderboard index", static_cast<int>(index));

    return &playerStats.at(playerOrder[index]);
}

std::vector<PlayerStats> Leaderboard::getPlayers()
{
    std::vector<PlayerStats> players;
    players.reserve(playerOrder.size());

    for (const std::string &name : playerOrder)
        players.push_back(playerStats.at(name));

    return players;
}

unsigned int Leaderboard::getPlayerCount()
{
    return static_cast<unsigned int>(playerOrder.size());
}

unsigned int Leaderboard::getOverallGames()
{
    return overallGames;
}

void Leaderboard::addGame()
{
    ++overallGames;
}

void Leaderboard::removePlayer(const std::string &name)
{
    if (name.empty())
        throw MinesweeperException("Player name cannot be empty");

    auto it = playerStats.find(name);
    if (it == playerStats.end())
        throw DatabaseException("Player not found in leaderboard", 0);

    if (overallGames >= static_cast<unsigned int>(it->second.getMatchesNumber()))
        overallGames -= static_cast<unsigned int>(it->second.getMatchesNumber());

    playerStats.erase(it);

    std::erase_if(playerOrder, [&](const std::string &playerName)
    {
        return playerName == name;
    });
}

void Leaderboard::sort(char order, bool equality)
{
    if (playerOrder.size() <= 1)
        return;

    if (order != 'a' && order != 'd')
        throw MinesweeperException("Invalid sort order");

    auto comparator = [order](const std::string &lhs, const std::string &rhs)
    {
        const PlayerStats &leftPlayer = playerStats.at(lhs);
        const PlayerStats &rightPlayer = playerStats.at(rhs);

        if (order == 'a')
            return leftPlayer < rightPlayer;
        return rightPlayer < leftPlayer;
    };

    if (equality)
        std::stable_sort(playerOrder.begin(), playerOrder.end(), comparator);
    else
        std::sort(playerOrder.begin(), playerOrder.end(), comparator);
}

/**
 * Alegem "cel mai bun" jucator dupa win rate, apoi dupa numarul de victorii.
 */
template<>
const PlayerStats *findBest<PlayerStats>(const std::vector<PlayerStats> &values)
{
    if (values.empty())
        return nullptr;

    // lhs = left-hand side, rhs = right-hand side
    auto it = std::max_element(values.begin(), values.end(), [](const PlayerStats &lhs, const PlayerStats &rhs)
    {
        if (lhs.getWinRate() != rhs.getWinRate())
            return lhs.getWinRate() < rhs.getWinRate();

        if (lhs.getWins() != rhs.getWins())
            return lhs.getWins() < rhs.getWins();

        return lhs.getPlayerName() < rhs.getPlayerName();
    });

    return &*it;
}
