
// Minesweeper headers
#include "game.hpp"
#include "cell.hpp"
#include "game_setup.hpp"
#include "map.hpp"
#include "stats.hpp"

// External libraries
#include "raylib.h"

// C++ standard library
#include <chrono>
#include <string>

class Render final
{
public:
    enum class GUIState
    {
        MENU,
        GAME,
        MY_STATS,
        LEADERBOARD,
        JOURNAL
    };

private:
    std::string mapWidthText;
    std::string mapHeightText;
    std::string mapBombsText;
    std::string mapSeedText;
    std::string playerName;
    std::string leaderboardMinGamesText;

    GUIState guiState;
    Game game;
    long long finalTime;
    bool statsSaved;
    bool mapRevealedOnLoss;

    Texture2D flagTexture;
    Texture2D bombTexture;

    Font AdwaitaMono;

    void DrawGame();
    void HandleGameInput();
    void DrawMenu();
    void HandleMenuInput();
    void DrawMyStats();
    void HandleMyStatsInput();
    void DrawLeaderboard();
    void HandleLeaderboardInput();
    void DrawJournal();
    void HandleJournalInput();

    void SaveMatchStats();

public:
    Render();
    // Render(Game game);
    ~Render();

    void StartRender();
    void InitializeGame(const GameSetup &setup);
};
