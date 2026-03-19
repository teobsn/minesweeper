
// Minesweeper headers
#include "game.hpp"
#include "cell.hpp"
#include "map.hpp"
#include "stats.hpp"

// External libraries
#include "raylib.h"

class Render
{
private:
    char mapWidthText[5];
    char mapHeightText[5];
    char mapBombsText[5];
    char mapSeedText[13];

    Game game;

public:
    Render();
    // Render(Game game);
    ~Render();

    void StartRender();
    void InitializeGame(unsigned int width, unsigned int height, unsigned int bombs, unsigned long long seed);
};