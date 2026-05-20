#include "game.hpp"
#include "powerups.hpp"
#include "stats.hpp"
#include "templates.hpp"

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
class TestSuite
{
private:
    std::size_t executed = 0;

public:
    void run(const std::string &name, const std::function<void()> &test)
    {
        ++executed;
        try
        {
            test();
        }
        catch (const std::exception &e)
        {
            std::cerr << "[FAIL] " << name << ": " << e.what() << "\n";
            std::exit(1);
        }
        catch (...)
        {
            std::cerr << "[FAIL] " << name << ": unknown exception\n";
            std::exit(1);
        }
    }

    std::size_t count() const noexcept
    {
        return executed;
    }
};

void fail(const std::string &message)
{
    throw std::runtime_error(message);
}

void check(bool condition, const std::string &message)
{
    if (!condition)
        fail(message);
}

Match makeMatch(unsigned int width, unsigned int height, unsigned long long seed, unsigned int bombs,
                unsigned int revealed, unsigned int clicks, int timeMs, bool won, bool finished)
{
    return Match(width, height, seed, bombs, revealed, clicks, std::chrono::duration<int, std::milli>(timeMs), won, finished);
}
}

int main()
{
    TestSuite suite;

    suite.run("StaticQueue", []
    {
        StaticQueue<int, 3> queue{1, 2};
        check(queue.size() == 2, "StaticQueue should keep initializer_list values");
        queue.push(3);
        check(queue.full(), "StaticQueue should report full after reaching capacity");
        check(queue[0] == 1 && queue[1] == 2 && queue[2] == 3, "StaticQueue should preserve insertion order");
        queue.pop();
        check(queue.size() == 2 && queue[0] == 2 && queue[1] == 3, "StaticQueue pop should remove the oldest value");

        auto strings = queue.template transform<std::string>([](int value)
                                                             { return std::to_string(value); });
        check(strings.size() == 2 && strings[0] == "2" && strings[1] == "3", "StaticQueue transform should map values");
    });

    suite.run("PlayerStats", []
    {
        PlayerStats stats("Alice");

        for (int i = 0; i < 11; ++i)
        {
            bool won = (i % 2 == 0);
            stats.addMatch(makeMatch(8, 8, static_cast<unsigned long long>(1000 + i), 8, static_cast<unsigned int>(i + 1),
                                     static_cast<unsigned int>(i + 2), 1000 * (i + 1), won, true));
        }

        check(stats.getMatchesNumber() == 11, "PlayerStats should keep the full match history");
        check(stats.getWins() == 6, "PlayerStats should count wins across the full history");
        check(std::abs(stats.getWinRate() - 6.0f / 11.0f) < 0.0001f, "PlayerStats should compute win rate from the full history");
        check(stats.getRecentMatches().size() == 10, "PlayerStats should keep only the 10 most recent matches");
        check(stats.getRecentMatches()[0].getClicks() == 3, "Recent history should drop the oldest match when full");
        check(stats.getRecentMatches()[9].getClicks() == 12, "Recent history should contain the latest match");

        stats.removeMatches(2);
        check(stats.getMatchesNumber() == 9, "Removing matches should shrink the full history");
        check(stats.getRecentMatches().size() == 9, "Removing matches should refresh the recent history");
        check(stats.getWins() == 5, "Removing matches should refresh win counting");
    });

    suite.run("ShieldPowerUp", []
    {
        Game shieldGame(Game::GameState::NOT_STARTED, Map(1, 1, 1234ULL, 1));
        auto *shield = new ShieldPowerUp(1, 1.0f);
        shield->use(shieldGame, 0, 0);
        check(shieldGame.isInvulnerable(), "Shield should set the game to invulnerable");
        shieldGame.revealCell(0, 0);
        check(shieldGame.getCurrentState() == Game::GameState::WON, "Shield should let the player survive the bomb");
        check(!shieldGame.isInvulnerable(), "Shield should be consumed after a bomb is revealed");
        check(shieldGame.getPowerUpCount("Shield") == 1, "Shield usage should be recorded in power-up history");
    });

    suite.run("ScannerPowerUp", []
    {
        Game scannerGame(Game::GameState::NOT_STARTED, Map(2, 2, 4321ULL, 0));
        auto *scanner = new ScannerPowerUp(2, 1);
        scanner->use(scannerGame, 0, 0);
        check(scannerGame.getPowerUpCount("Scanner") == 1, "Scanner usage should be recorded");
        check(scannerGame.getMap().isRevealedAt(0, 0), "Scanner should reveal the targeted area");
        check(scannerGame.getCurrentState() == Game::GameState::WON, "Scanner should finish a zero-bomb board");
    });

    suite.run("UltimatePowerUp", []
    {
        Game ultimateGame(Game::GameState::NOT_STARTED, Map(2, 2, 9999ULL, 0));
        auto *ultimate = new UltimatePowerUp(3, 10.0f, 1);
        ultimate->use(ultimateGame, 1, 1);
        check(ultimateGame.getPowerUpCount("Ultimate") == 1, "Ultimate usage should be recorded");
        check(ultimateGame.isInvulnerable(), "Ultimate should enable invulnerability");
        check(ultimateGame.getMap().isRevealedAt(1, 1), "Ultimate should reveal the targeted area");
        check(ultimateGame.getCurrentState() == Game::GameState::WON, "Ultimate should finish a zero-bomb board");
    });

    suite.run("BombDefuser", []
    {
        Game defuserGame(Game::GameState::NOT_STARTED, Map(2, 2, 7777ULL, 4));
        auto *defuser = new BombDefuser(4, 0.5);
        unsigned int bombsBefore = defuserGame.getMap().getBombs();
        defuser->use(defuserGame, 0, 0);
        unsigned int bombsAfter = defuserGame.getMap().getBombs();
        check(bombsBefore == 4, "Bomb defuser test should start from a fully mined board");
        check(bombsAfter < bombsBefore, "Bomb defuser should reduce the number of bombs");
        check(defuserGame.getPowerUpCount("Defuser") == 1, "Bomb defuser usage should be recorded");
    });

    suite.run("findBest", []
    {
        PlayerStats best("Best");
        PlayerStats worse("Worse");

        best.addMatch(makeMatch(8, 8, 1, 8, 10, 3, 1000, true, true));
        best.addMatch(makeMatch(8, 8, 2, 8, 10, 3, 1000, true, true));
        worse.addMatch(makeMatch(8, 8, 3, 8, 10, 3, 1000, false, true));

        std::vector<PlayerStats> players{worse, best};
        const PlayerStats *picked = findBest<PlayerStats>(players);
        check(picked != nullptr, "findBest should return a valid player");
        check(picked->getPlayerName() == "Best", "findBest should pick the highest win rate player");
    });

    std::cout << "[OK] " << suite.count() << " unit test groups passed\n";
    return 0;
}
