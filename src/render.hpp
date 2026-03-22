
// Minesweeper headers
#include "game.hpp"
#include "cell.hpp"
#include "map.hpp"
#include "stats.hpp"

// External libraries
#include "raylib.h"

// C++ standard library
#include <chrono>

class Render
{
public:
    enum class GUIState {
        MENU,
        GAME,
        MY_STATS,
        LEADERBOARD
    };

private:
    char mapWidthText[5];
    char mapHeightText[5];
    char mapBombsText[5];
    char mapSeedText[13];
    char playerNameText[64];

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

    void SaveMatchStats();

public:
    Render();
    // Render(Game game);
    ~Render();

    void StartRender();
    void InitializeGame(unsigned int width, unsigned int height, unsigned int bombs, unsigned long long seed);
};