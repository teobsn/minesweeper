
// Minesweeper headers
#include "game.hpp"
#include "cell.hpp"
#include "map.hpp"
#include "stats.hpp"
#include "render.hpp"
#include "powerups.hpp"

// External libraries
#include "raylib.h"

// C++ standard library
#include <iostream>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>

Render::Render()
{
    strcpy(mapWidthText, "12");
    strcpy(mapHeightText, "12");
    strcpy(mapBombsText, "8");
    strcpy(mapSeedText, "");
    strcpy(playerNameText, "Player");
    finalTime = 0;
    guiState = GUIState::MENU;
    statsSaved = false;
    mapRevealedOnLoss = false;
}

Render::~Render()
{
}

void Render::InitializeGame(unsigned int width, unsigned int height, unsigned int bombs, unsigned long long seed)
{
    std::cout << "Generating map with attributes:\n";
    std::cout << "Width: " << width << "\n";
    std::cout << "Height: " << height << "\n";
    std::cout << "Number of bombs: " << bombs << "\n";
    std::cout << "Seed: " << seed << "\n";

    Map newMap(width, height, seed, bombs);
    game = Game(Game::GameState::NOT_STARTED, newMap);
    guiState = GUIState::GAME;
    statsSaved = false;
    mapRevealedOnLoss = false;

    // Resize window based on map size
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    int minWidth = (int)width * 30 + 40;
    int minHeight = (int)height * 30 + 100;
    if (screenWidth < minWidth || screenHeight < minHeight) {
        SetWindowSize(std::max(screenWidth, minWidth), std::max(screenHeight, minHeight));
    }
}

void Render::StartRender()
{
    game = Game(Game::GameState::MENU);
    guiState = GUIState::MENU;

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);

    const int screenWidth = 800;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Minesweeper");
    SetTargetFPS(60);

    AdwaitaMono = LoadFont("resources/fonts/adw/AdwaitaMono-Regular.ttf");
    flagTexture = LoadTexture("resources/textures/flag.png");
    bombTexture = LoadTexture("resources/textures/bomb.png");

    while (!WindowShouldClose())
    {
        switch (guiState)
        {
        case GUIState::MENU:
            HandleMenuInput();
            BeginDrawing();
            ClearBackground(Color{ 12, 12, 12, 255 });
            DrawMenu();
            EndDrawing();
            break;
        case GUIState::GAME:
            HandleGameInput();
            BeginDrawing();
            ClearBackground(Color{ 12, 12, 12, 255 });
            DrawGame();
            EndDrawing();
            break;
        case GUIState::MY_STATS:
            HandleMyStatsInput();
            BeginDrawing();
            ClearBackground(Color{ 12, 12, 12, 255 });
            DrawMyStats();
            EndDrawing();
            break;
        case GUIState::LEADERBOARD:
            HandleLeaderboardInput();
            BeginDrawing();
            ClearBackground(Color{ 12, 12, 12, 255 });
            DrawLeaderboard();
            EndDrawing();
            break;
        }
    }

    UnloadFont(AdwaitaMono);
    UnloadTexture(flagTexture);
    UnloadTexture(bombTexture);
    
    CloseWindow();
}

void Render::DrawMenu()
{
    const float padding = 20.0f;
    const float textSize = 32.0f;
    const float rectSize = 40.0f;
    const int screenWidth = GetScreenWidth();

    const Color textColor = WHITE;
    const Color rectColor = DARKGRAY;

    float renderPosX = padding, renderPosY = padding, offsetX = screenWidth / 2.0f;
    float sizeX = screenWidth / 2.0f - 2.0f * padding;

    DrawTextEx(AdwaitaMono, "MINESWEEPER", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "MINESWEEPER", 48, 0).x / 2.0f, renderPosY}, 48, 0, RED);
    renderPosY += 80.0f;

    DrawTextEx(AdwaitaMono, "Player Name", {renderPosX, renderPosY}, textSize, 0, textColor);
    Rectangle textBoxPlayerName = {offsetX, renderPosY, sizeX, rectSize};
    DrawRectangleRec(textBoxPlayerName, rectColor);
    
    // const_cast;
    const char* displayName = playerNameText;
    char* modifiableName = const_cast<char*>(displayName);
    DrawTextEx(AdwaitaMono, modifiableName, {textBoxPlayerName.x + 5, textBoxPlayerName.y + 5}, textSize, 0, textColor);

    renderPosY += 60.0f;
    DrawTextEx(AdwaitaMono, "Map Width", {renderPosX, renderPosY}, textSize, 0, textColor);
    Rectangle textBoxMapWidth = {offsetX, renderPosY, sizeX, rectSize};
    DrawRectangleRec(textBoxMapWidth, rectColor);
    DrawTextEx(AdwaitaMono, mapWidthText, {textBoxMapWidth.x + 5, textBoxMapWidth.y + 5}, textSize, 0, textColor);

    renderPosY += 60.0f;
    DrawTextEx(AdwaitaMono, "Map Height", {renderPosX, renderPosY}, textSize, 0, textColor);
    Rectangle textBoxMapHeight = {offsetX, renderPosY, sizeX, rectSize};
    DrawRectangleRec(textBoxMapHeight, rectColor);
    DrawTextEx(AdwaitaMono, mapHeightText, {textBoxMapHeight.x + 5, textBoxMapHeight.y + 5}, textSize, 0, textColor);

    renderPosY += 60.0f;
    DrawTextEx(AdwaitaMono, "Bombs", {renderPosX, renderPosY}, textSize, 0, textColor);
    Rectangle textBoxMapBombs = {offsetX, renderPosY, sizeX, rectSize};
    DrawRectangleRec(textBoxMapBombs, rectColor);
    DrawTextEx(AdwaitaMono, mapBombsText, {textBoxMapBombs.x + 5, textBoxMapBombs.y + 5}, textSize, 0, textColor);

    renderPosY += 60.0f;
    DrawTextEx(AdwaitaMono, "Seed", {renderPosX, renderPosY}, textSize, 0, textColor);
    Rectangle textBoxMapSeed = {offsetX, renderPosY, sizeX, rectSize};
    DrawRectangleRec(textBoxMapSeed, rectColor);
    if (strlen(mapSeedText) == 0) {
        DrawTextEx(AdwaitaMono, "(optional)", {textBoxMapSeed.x + 5, textBoxMapSeed.y + 5}, textSize, 0, GRAY);
    } else {
        DrawTextEx(AdwaitaMono, mapSeedText, {textBoxMapSeed.x + 5, textBoxMapSeed.y + 5}, textSize, 0, textColor);
    }

    renderPosY += 80.0f;
    Rectangle startButton = {screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    DrawRectangleRec(startButton, GREEN);
    DrawTextEx(AdwaitaMono, "START GAME", {startButton.x + 150.0f - MeasureTextEx(AdwaitaMono, "START GAME", textSize, 0).x / 2.0f, startButton.y + 10.0f}, textSize, 0, BLACK);

    renderPosY += 70.0f;
    Rectangle myStatsButton = {screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    DrawRectangleRec(myStatsButton, BLUE);
    DrawTextEx(AdwaitaMono, "MY STATISTICS", {myStatsButton.x + 150.0f - MeasureTextEx(AdwaitaMono, "MY STATISTICS", textSize, 0).x / 2.0f, myStatsButton.y + 10.0f}, textSize, 0, WHITE);

    renderPosY += 70.0f;
    Rectangle leaderboardButton = {screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    DrawRectangleRec(leaderboardButton, PURPLE);
    DrawTextEx(AdwaitaMono, "LEADERBOARD", {leaderboardButton.x + 150.0f - MeasureTextEx(AdwaitaMono, "LEADERBOARD", textSize, 0).x / 2.0f, leaderboardButton.y + 10.0f}, textSize, 0, WHITE);

    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, textBoxPlayerName)) DrawRectangleLinesEx(textBoxPlayerName, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, textBoxMapWidth)) DrawRectangleLinesEx(textBoxMapWidth, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, textBoxMapHeight)) DrawRectangleLinesEx(textBoxMapHeight, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, textBoxMapBombs)) DrawRectangleLinesEx(textBoxMapBombs, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, textBoxMapSeed)) DrawRectangleLinesEx(textBoxMapSeed, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, startButton)) DrawRectangleLinesEx(startButton, 2, WHITE);
    if (CheckCollisionPointRec(mousePos, myStatsButton)) DrawRectangleLinesEx(myStatsButton, 2, WHITE);
    if (CheckCollisionPointRec(mousePos, leaderboardButton)) DrawRectangleLinesEx(leaderboardButton, 2, WHITE);
}

void Render::HandleMenuInput()
{
    Vector2 mousePos = GetMousePosition();
    const float padding = 20.0f;
    const float rectSize = 40.0f;
    const int screenWidth = GetScreenWidth();
    const float offsetX = (float)screenWidth / 2.0f;
    const float sizeX = (float)screenWidth / 2.0f - 2.0f * padding;
    float renderPosY = 100.0f; 

    Rectangle textBoxPlayerName = {offsetX, renderPosY, sizeX, rectSize};
    renderPosY += 60.0f;
    Rectangle textBoxMapWidth = {offsetX, renderPosY, sizeX, rectSize};
    renderPosY += 60.0f;
    Rectangle textBoxMapHeight = {offsetX, renderPosY, sizeX, rectSize};
    renderPosY += 60.0f;
    Rectangle textBoxMapBombs = {offsetX, renderPosY, sizeX, rectSize};
    renderPosY += 60.0f;
    Rectangle textBoxMapSeed = {offsetX, renderPosY, sizeX, rectSize};
    renderPosY += 80.0f;
    Rectangle startButton = {(float)screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    renderPosY += 70.0f;
    Rectangle myStatsButton = {(float)screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    renderPosY += 70.0f;
    Rectangle leaderboardButton = {(float)screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};

    auto handleTextInput = [](char* text, int maxLen, bool numericOnly) {
        int key = GetCharPressed();
        while (key > 0) {
            bool validKey = !numericOnly || (key >= 48 && key <= 57);
            if (validKey && ((int)strlen(text) < maxLen)) {
                int len = (int)strlen(text);
                text[len] = (char)key;
                text[len + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = (int)strlen(text);
            if (len > 0) text[len - 1] = '\0';
        }
    };

    if (CheckCollisionPointRec(mousePos, textBoxPlayerName)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        handleTextInput(playerNameText, 63, false);
    } else if (CheckCollisionPointRec(mousePos, textBoxMapWidth)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        handleTextInput(mapWidthText, 2, true);
    } else if (CheckCollisionPointRec(mousePos, textBoxMapHeight)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        handleTextInput(mapHeightText, 2, true);
    } else if (CheckCollisionPointRec(mousePos, textBoxMapBombs)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        handleTextInput(mapBombsText, 4, true);
    } else if (CheckCollisionPointRec(mousePos, textBoxMapSeed)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        handleTextInput(mapSeedText, 12, true);
    } else if (CheckCollisionPointRec(mousePos, startButton)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            unsigned int w = (unsigned int)atoi(mapWidthText);
            unsigned int h = (unsigned int)atoi(mapHeightText);
            if (w > 100) w = 100;
            if (h > 100) h = 100;
            unsigned int b = (unsigned int)atoi(mapBombsText);
            unsigned long long s = 0;
            if (strlen(mapSeedText) > 0) {
                try { s = std::stoull(mapSeedText); } catch (...) { s = (unsigned long long)time(nullptr); }
            } else s = (unsigned long long)time(nullptr);
            if (w > 0 && h > 0) InitializeGame(w, h, b, s);
        }
    } else if (CheckCollisionPointRec(mousePos, myStatsButton)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) guiState = GUIState::MY_STATS;
    } else if (CheckCollisionPointRec(mousePos, leaderboardButton)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) guiState = GUIState::LEADERBOARD;
    } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void Render::DrawGame()
{
    // Capture reference to avoid copies
    const Map& map = game.getMap();
    unsigned int width = map.getWidth();
    unsigned int height = map.getHeight();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Game::GameState state = game.getCurrentState();
    if (state == Game::GameState::WON || state == Game::GameState::LOST) {
        if (finalTime == 0) finalTime = game.getElapsedTime().count();
        SaveMatchStats();
        if (state == Game::GameState::LOST && !mapRevealedOnLoss) {
            game.revealFullMap();
            mapRevealedOnLoss = true;
        }
    } else if (state == Game::GameState::IN_PROGRESS || state == Game::GameState::NOT_STARTED) {
        finalTime = 0;
    }

    float statsHeight = 60.0f;
    DrawRectangle(0, 0, screenWidth, (int)statsHeight, DARKGRAY);
    
    long long displayTime = finalTime > 0 ? finalTime : game.getElapsedTime().count();
    std::string timeStr;
    if (displayTime < 60000) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(3) << "Time: " << (displayTime / 1000.0) << "s";
        timeStr = ss.str();
    } else timeStr = "Time: " + std::to_string(displayTime / 1000) + "s";
    
    std::string flagsStr = "Flags: " + std::to_string(game.getFlagsPlaced()) + "/" + std::to_string(map.getBombs());
    DrawTextEx(AdwaitaMono, timeStr.c_str(), {20.0f, 15.0f}, 30.0f, 0, WHITE);
    DrawTextEx(AdwaitaMono, flagsStr.c_str(), {(float)screenWidth - MeasureTextEx(AdwaitaMono, flagsStr.c_str(), 30, 0).x - 20.0f, 15.0f}, 30.0f, 0, WHITE);

    if (game.isInvulnerable()) {
        DrawTextEx(AdwaitaMono, "SHIELD ACTIVE", {(float)screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "SHIELD ACTIVE", 20, 0).x / 2.0f, 45.0f}, 20.0f, 0, GOLD);
    }

    if (game.getCurrentState() == Game::GameState::WON) {
        DrawTextEx(AdwaitaMono, "YOU WON!", {(float)screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "YOU WON!", 30, 0).x / 2.0f, 15.0f}, 30.0f, 0, GREEN);
    } else if (game.getCurrentState() == Game::GameState::LOST) {
        DrawTextEx(AdwaitaMono, "GAME OVER", {(float)screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "GAME OVER", 30, 0).x / 2.0f, 15.0f}, 30.0f, 0, RED);
    }

    float gridPadding = 20.0f;
    float availableWidth = (float)screenWidth - 2.0f * gridPadding;
    float availableHeight = (float)screenHeight - statsHeight - 2.0f * gridPadding;
    float cellSize = std::min(availableWidth / (float)width, availableHeight / (float)height);
    float startX = gridPadding + (availableWidth - (float)width * cellSize) / 2.0f;
    float startY = statsHeight + gridPadding + (availableHeight - (float)height * cellSize) / 2.0f;

    Color numberColors[] = { BLACK, BLUE, GREEN, RED, PURPLE, MAROON, SKYBLUE, DARKGRAY, GOLD };

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            Rectangle cellRect = {startX + (float)x * cellSize, startY + (float)y * cellSize, cellSize, cellSize};
            if (map.isRevealedAt(x, y)) {
                DrawRectangleRec(cellRect, LIGHTGRAY);
                if (map.isBombAt(x, y)) {
                    DrawRectangleRec(cellRect, RED);
                    DrawTexturePro(bombTexture, {0, 0, (float)bombTexture.width, (float)bombTexture.height}, cellRect, {0, 0}, 0, WHITE);
                } else {
                    unsigned int adj = map.getAdjacentBombsAt(x, y);
                    if (adj > 0) {
                        std::string adjStr = std::to_string(adj);
                        Vector2 textSize = MeasureTextEx(AdwaitaMono, adjStr.c_str(), cellSize * 0.8f, 0);
                        DrawTextEx(AdwaitaMono, adjStr.c_str(), {cellRect.x + (cellSize - textSize.x) / 2.0f, cellRect.y + (cellSize - textSize.y) / 2.0f}, cellSize * 0.8f, 0, numberColors[adj]);
                    }
                }
            } else {
                DrawRectangleRec(cellRect, GRAY);
                if (map.isFlaggedAt(x, y)) DrawTexturePro(flagTexture, {0, 0, (float)flagTexture.width, (float)flagTexture.height}, cellRect, {0, 0}, 0, WHITE);
            }
            DrawRectangleLinesEx(cellRect, 1, DARKGRAY);
        }
    }
    // Interface buttons
    Vector2 mousePos = GetMousePosition();
    
    // Left side buttons: +, *0.75, *1.5
    Rectangle addBombButton = {20.0f, (float)screenHeight - 40.0f, 40.0f, 30.0f};
    DrawRectangleRec(addBombButton, DARKGRAY);
    DrawTextEx(AdwaitaMono, "+", {addBombButton.x + 20.0f - MeasureTextEx(AdwaitaMono, "+", 20, 0).x / 2.0f, addBombButton.y + 5.0f}, 20.0f, 0, WHITE);
    if (CheckCollisionPointRec(mousePos, addBombButton)) DrawRectangleLinesEx(addBombButton, 2, WHITE);

    Rectangle scaleDownButton = {65.0f, (float)screenHeight - 40.0f, 60.0f, 30.0f};
    DrawRectangleRec(scaleDownButton, DARKGRAY);
    DrawTextEx(AdwaitaMono, "*0.75", {scaleDownButton.x + 30.0f - MeasureTextEx(AdwaitaMono, "*0.75", 20, 0).x / 2.0f, scaleDownButton.y + 5.0f}, 20.0f, 0, WHITE);
    if (CheckCollisionPointRec(mousePos, scaleDownButton)) DrawRectangleLinesEx(scaleDownButton, 2, WHITE);

    Rectangle scaleBombsButton = {130.0f, (float)screenHeight - 40.0f, 60.0f, 30.0f};
    DrawRectangleRec(scaleBombsButton, DARKGRAY);
    DrawTextEx(AdwaitaMono, "*1.5", {scaleBombsButton.x + 30.0f - MeasureTextEx(AdwaitaMono, "*1.5", 20, 0).x / 2.0f, scaleBombsButton.y + 5.0f}, 20.0f, 0, WHITE);
    if (CheckCollisionPointRec(mousePos, scaleBombsButton)) DrawRectangleLinesEx(scaleBombsButton, 2, WHITE);

    // Power-up buttons
    Rectangle shieldBtn = {200.0f, (float)screenHeight - 40.0f, 80.0f, 30.0f};
    DrawRectangleRec(shieldBtn, DARKBLUE);
    DrawTextEx(AdwaitaMono, "SHIELD", {shieldBtn.x + 40.0f - MeasureTextEx(AdwaitaMono, "SHIELD", 18, 0).x / 2.0f, shieldBtn.y + 6.0f}, 18.0f, 0, WHITE);
    if (CheckCollisionPointRec(mousePos, shieldBtn)) {
        DrawRectangleLinesEx(shieldBtn, 2, GOLD);
        BasePowerUp* temp = new ShieldPowerUp(0, 0);
        DrawTextEx(AdwaitaMono, temp->getDescription().c_str(), {shieldBtn.x, shieldBtn.y - 25.0f}, 16, 0, GOLD);
        delete temp;
    }

    Rectangle ultiBtn = {290.0f, (float)screenHeight - 40.0f, 80.0f, 30.0f};
    DrawRectangleRec(ultiBtn, MAROON);
    DrawTextEx(AdwaitaMono, "ULTI", {ultiBtn.x + 40.0f - MeasureTextEx(AdwaitaMono, "ULTI", 18, 0).x / 2.0f, ultiBtn.y + 6.0f}, 18.0f, 0, WHITE);
    if (CheckCollisionPointRec(mousePos, ultiBtn)) {
        DrawRectangleLinesEx(ultiBtn, 2, PURPLE);
        BasePowerUp* temp = new UltimatePowerUp(0, 0, 0);
        DrawTextEx(AdwaitaMono, temp->getDescription().c_str(), {ultiBtn.x, ultiBtn.y - 25.0f}, 16, 0, PURPLE);
        delete temp;
    }

    Rectangle defuseBtn = {380.0f, (float)screenHeight - 40.0f, 80.0f, 30.0f};
    DrawRectangleRec(defuseBtn, DARKPURPLE);
    DrawTextEx(AdwaitaMono, "DEFUSE", {defuseBtn.x + 40.0f - MeasureTextEx(AdwaitaMono, "DEFUSE", 18, 0).x / 2.0f, defuseBtn.y + 6.0f}, 18.0f, 0, WHITE);
    if (CheckCollisionPointRec(mousePos, defuseBtn)) {
        DrawRectangleLinesEx(defuseBtn, 2, WHITE);
        BasePowerUp* temp = new BombDefuser(0, 0);
        DrawTextEx(AdwaitaMono, temp->getDescription().c_str(), {defuseBtn.x, defuseBtn.y - 25.0f}, 16, 0, WHITE);
        delete temp;
    }

    // Right side button: MENU
    Rectangle backButton = {(float)screenWidth - 220.0f, (float)screenHeight - 40.0f, 200.0f, 30.0f};
    DrawRectangleRec(backButton, DARKGRAY);
    DrawTextEx(AdwaitaMono, "MENU", {backButton.x + 100.0f - MeasureTextEx(AdwaitaMono, "MENU", 20, 0).x / 2.0f, backButton.y + 5.0f}, 20.0f, 0, WHITE);
    if (CheckCollisionPointRec(mousePos, backButton)) DrawRectangleLinesEx(backButton, 2, WHITE);
}

void Render::HandleGameInput()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    Vector2 mousePos = GetMousePosition();

    Rectangle backButton = {(float)screenWidth - 220.0f, (float)screenHeight - 40.0f, 200.0f, 30.0f};
    Rectangle addBombButton = {20.0f, (float)screenHeight - 40.0f, 40.0f, 30.0f};
    Rectangle scaleDownButton = {65.0f, (float)screenHeight - 40.0f, 60.0f, 30.0f};
    Rectangle scaleBombsButton = {130.0f, (float)screenHeight - 40.0f, 60.0f, 30.0f};
    Rectangle shieldBtn = {200.0f, (float)screenHeight - 40.0f, 80.0f, 30.0f};
    Rectangle ultiBtn = {290.0f, (float)screenHeight - 40.0f, 80.0f, 30.0f};
    Rectangle defuseBtn = {380.0f, (float)screenHeight - 40.0f, 80.0f, 30.0f};

    const Map& map = game.getMap();
    unsigned int width = map.getWidth();
    unsigned int height = map.getHeight();
    float statsHeight = 60.0f;
    float gridPadding = 20.0f;
    float availableWidth = (float)screenWidth - 2.0f * gridPadding;
    float availableHeight = (float)screenHeight - statsHeight - 2.0f * gridPadding;
    float cellSize = std::min(availableWidth / (float)width, availableHeight / (float)height);
    float startX = gridPadding + (availableWidth - (float)width * cellSize) / 2.0f;
    float startY = statsHeight + gridPadding + (availableHeight - (float)height * cellSize) / 2.0f;

    int mouseGridX = (int)((mousePos.x - startX) / cellSize);
    int mouseGridY = (int)((mousePos.y - startY) / cellSize);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePos, backButton)) {
            guiState = GUIState::MENU;
            return;
        }
        if (CheckCollisionPointRec(mousePos, addBombButton)) {
            game + 1;
            return;
        }
        if (CheckCollisionPointRec(mousePos, scaleDownButton)) {
            game * 0.75;
            return;
        }
        if (CheckCollisionPointRec(mousePos, scaleBombsButton)) {
            game * 1.5;
            return;
        }
        if (CheckCollisionPointRec(mousePos, shieldBtn)) {
            BasePowerUp* p = new ShieldPowerUp(1, 15.0f);
            p->use(game, mouseGridX, mouseGridY);
            delete p;
            return;
        }
        if (CheckCollisionPointRec(mousePos, ultiBtn)) {
            BasePowerUp* p = new UltimatePowerUp(3, 10.0f, 5);
            p->use(game, mouseGridX, mouseGridY);
            delete p;
            return;
        }
        if (CheckCollisionPointRec(mousePos, defuseBtn)) {
            BasePowerUp* p = new BombDefuser(4, 0.5);
            p->use(game, mouseGridX, mouseGridY);
            delete p;
            return;
        }
    }

    if (mousePos.x >= startX && mousePos.x < startX + (float)width * cellSize && mousePos.y >= startY && mousePos.y < startY + (float)height * cellSize) {
        if (game.getCurrentState() == Game::GameState::IN_PROGRESS || game.getCurrentState() == Game::GameState::NOT_STARTED) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (game.getCurrentState() == Game::GameState::NOT_STARTED) {
                    unsigned int w = map.getWidth();
                    unsigned int h = map.getHeight();
                    unsigned int b = map.getBombs();
                    unsigned long long s = map.getSeed();
                    if (game.getMap().isBombAt(mouseGridX, mouseGridY)) {
                        s++;
                        game.setMap(Map(w, h, s, b));
                    }
                    game.startGame();
                }
                game.revealCell(mouseGridX, mouseGridY);
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) game.toggleFlag(mouseGridX, mouseGridY);
            else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) game.adjacentReveal(mouseGridX, mouseGridY);
        }
    }
    if (IsKeyPressed(KEY_R)) game.reset();
    if (IsKeyPressed(KEY_ESCAPE)) guiState = GUIState::MENU;

    if (IsKeyPressed(KEY_ONE)) {
        BasePowerUp* p = new ShieldPowerUp(1, 15.0f);
        p->use(game, mouseGridX, mouseGridY);
        delete p;
    }
    if (IsKeyPressed(KEY_TWO)) {
        BasePowerUp* p = new ScannerPowerUp(2, 3);
        p->use(game, mouseGridX, mouseGridY);
        delete p;
    }
    if (IsKeyPressed(KEY_THREE)) {
        BasePowerUp* p = new UltimatePowerUp(3, 10.0f, 5);
        p->use(game, mouseGridX, mouseGridY);
        delete p;
    }
    if (IsKeyPressed(KEY_FOUR)) {
        BasePowerUp* p = new BombDefuser(4, 0.5);
        p->use(game, mouseGridX, mouseGridY);
        delete p;
    }
}

void Render::DrawMyStats()
{
    const int screenWidth = GetScreenWidth();
    const float padding = 40.0f;
    float renderPosY = padding;

    DrawTextEx(AdwaitaMono, "MY STATISTICS", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "MY STATISTICS", 48, 0).x / 2.0f, renderPosY}, 48, 0, BLUE);
    renderPosY += 70.0f;

    PlayerStats *player = Leaderboard::findPlayer(playerNameText);
    if (!player || player->getMatchesNumber() == 0) {
        DrawTextEx(AdwaitaMono, "No games found for this player.", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "No games found for this player.", 24, 0).x / 2.0f, renderPosY + 100.0f}, 24, 0, WHITE);
    } else {
        // Two-column layout for general stats
        float colWidth = (screenWidth - 2.0f * padding) / 2.0f;
        
        // Column 1
        std::stringstream ss1;
        ss1 << "Player: " << player->getPlayerName() << "\n";
        ss1 << "Total Games: " << player->getMatchesNumber() << "\n";
        ss1 << "Win Rate: " << std::fixed << std::setprecision(1) << player->getWinRate() * 100.0f << "%";
        DrawTextEx(AdwaitaMono, ss1.str().c_str(), {padding, renderPosY}, 24, 0, WHITE);

        // Column 2
        std::stringstream ss2;
        ss2 << "Wins: " << player->getWins() << "\n";
        ss2 << "Losses: " << (player->getMatchesNumber() - player->getWins()) << "\n";
        
        unsigned int totalTime = 0;
        unsigned int totalClicks = 0;
        const Match *matches = player->getMatches();
        int totalGames = player->getMatchesNumber();
        for (int i = 0; i < totalGames; ++i) {
            totalTime += matches[i].getTimePlayed().count();
            totalClicks += matches[i].getClicks();
        }
        ss2 << "Total Time: " << (totalTime / 1000) << "s\n";
        ss2 << "Avg Clicks: " << (totalGames > 0 ? totalClicks / totalGames : 0);
        DrawTextEx(AdwaitaMono, ss2.str().c_str(), {padding + colWidth, renderPosY}, 24, 0, WHITE);

        renderPosY += 140.0f;

        // Recent Games Table
        DrawTextEx(AdwaitaMono, "RECENT GAMES", {padding, renderPosY}, 28, 0, YELLOW);
        renderPosY += 40.0f;

        // Table Legend
        float resW = 100, sizeW = 120, bombsW = 100, timeW = 100;
        DrawTextEx(AdwaitaMono, "Result", {padding, renderPosY}, 20, 0, GRAY);
        DrawTextEx(AdwaitaMono, "Map Size", {padding + resW, renderPosY}, 20, 0, GRAY);
        DrawTextEx(AdwaitaMono, "Bombs", {padding + resW + sizeW, renderPosY}, 20, 0, GRAY);
        DrawTextEx(AdwaitaMono, "Time", {padding + resW + sizeW + bombsW, renderPosY}, 20, 0, GRAY);
        DrawTextEx(AdwaitaMono, "Clicks", {padding + resW + sizeW + bombsW + timeW, renderPosY}, 20, 0, GRAY);
        
        renderPosY += 30.0f;
        DrawRectangle(padding, renderPosY - 5, screenWidth - 2 * padding, 2, DARKGRAY);

        int start = std::max(0, totalGames - 12);
        for (int i = totalGames - 1; i >= start; --i) {
            Color resColor = matches[i].getWinState() ? GREEN : RED;
            DrawTextEx(AdwaitaMono, matches[i].getWinState() ? "WIN" : "LOSS", {padding, renderPosY}, 20, 0, resColor);
            
            std::string sizeStr = std::to_string(matches[i].getMapWidth()) + "x" + std::to_string(matches[i].getMapHeight());
            DrawTextEx(AdwaitaMono, sizeStr.c_str(), {padding + resW, renderPosY}, 20, 0, WHITE);
            DrawTextEx(AdwaitaMono, std::to_string(matches[i].getMapBombs()).c_str(), {padding + resW + sizeW, renderPosY}, 20, 0, WHITE);
            DrawTextEx(AdwaitaMono, (std::to_string(matches[i].getTimePlayed().count() / 1000) + "s").c_str(), {padding + resW + sizeW + bombsW, renderPosY}, 20, 0, WHITE);
            DrawTextEx(AdwaitaMono, std::to_string(matches[i].getClicks()).c_str(), {padding + resW + sizeW + bombsW + timeW, renderPosY}, 20, 0, WHITE);
            
            renderPosY += 25.0f;
        }
    }

    Rectangle removeLastButton = {padding, (float)GetScreenHeight() - 60.0f, 250.0f, 40.0f};
    DrawRectangleRec(removeLastButton, RED);
    DrawTextEx(AdwaitaMono, "REMOVE LAST MATCH", {removeLastButton.x + 125.0f - MeasureTextEx(AdwaitaMono, "REMOVE LAST MATCH", 20, 0).x / 2.0f, removeLastButton.y + 10.0f}, 20.0f, 0, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), removeLastButton)) DrawRectangleLinesEx(removeLastButton, 2, WHITE);

    Rectangle backButton = {(float)screenWidth - padding - 200.0f, (float)GetScreenHeight() - 60.0f, 200.0f, 40.0f};
    DrawRectangleRec(backButton, DARKGRAY);
    DrawTextEx(AdwaitaMono, "BACK", {backButton.x + 100.0f - MeasureTextEx(AdwaitaMono, "BACK", 24, 0).x / 2.0f, backButton.y + 8.0f}, 24, 0, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), backButton)) DrawRectangleLinesEx(backButton, 2, WHITE);
}

void Render::HandleMyStatsInput()
{
    const float padding = 40.0f;
    Vector2 mousePos = GetMousePosition();

    if (IsKeyPressed(KEY_ESCAPE)) guiState = GUIState::MENU;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Rectangle removeLastButton = {padding, (float)GetScreenHeight() - 60.0f, 250.0f, 40.0f};
        Rectangle backButton = {(float)GetScreenWidth() - padding - 200.0f, (float)GetScreenHeight() - 60.0f, 200.0f, 40.0f};
        
        if (CheckCollisionPointRec(mousePos, removeLastButton)) {
            PlayerStats *player = Leaderboard::findPlayer(playerNameText);
            if (player && player->getMatchesNumber() > 0) {
                --(*player);
            }
        }
        if (CheckCollisionPointRec(mousePos, backButton)) guiState = GUIState::MENU;
    }
}

void Render::DrawLeaderboard()
{
    const int screenWidth = GetScreenWidth();
    const float padding = 20.0f;
    float renderPosY = padding;

    DrawTextEx(AdwaitaMono, "LEADERBOARD", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "LEADERBOARD", 48, 0).x / 2.0f, renderPosY}, 48, 0, PURPLE);
    renderPosY += 70.0f;

    Leaderboard::sort();
    unsigned int playerCount = Leaderboard::getPlayerCount();

    if (playerCount == 0) {
        DrawTextEx(AdwaitaMono, "No players yet. Play some games first!", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "No players yet. Play some games first!", 24, 0).x / 2.0f, renderPosY + 100.0f}, 24, 0, WHITE);
    } else {
        float nameW = 200, gamesW = 80, winsW = 80, rateW = 100;
        DrawTextEx(AdwaitaMono, "Player", {padding, renderPosY}, 20, 0, YELLOW);
        DrawTextEx(AdwaitaMono, "Games", {padding + nameW, renderPosY}, 20, 0, YELLOW);
        DrawTextEx(AdwaitaMono, "Wins", {padding + nameW + gamesW, renderPosY}, 20, 0, YELLOW);
        DrawTextEx(AdwaitaMono, "Win Rate", {padding + nameW + gamesW + winsW, renderPosY}, 20, 0, YELLOW);
        DrawTextEx(AdwaitaMono, "Time", {padding + nameW + gamesW + winsW + rateW, renderPosY}, 20, 0, YELLOW);
        renderPosY += 30.0f;

        for (unsigned int i = 0; i < playerCount; ++i) {
            PlayerStats *p = Leaderboard::getPlayer(i);
            bool isCurrent = (strcmp(p->getPlayerName(), playerNameText) == 0);
            Color col = isCurrent ? GREEN : WHITE;

            DrawTextEx(AdwaitaMono, p->getPlayerName(), {padding, renderPosY}, 20, 0, col);
            DrawTextEx(AdwaitaMono, std::to_string(p->getMatchesNumber()).c_str(), {padding + nameW, renderPosY}, 20, 0, col);
            DrawTextEx(AdwaitaMono, std::to_string(p->getWins()).c_str(), {padding + nameW + gamesW, renderPosY}, 20, 0, col);
            
            std::stringstream rss;
            rss << std::fixed << std::setprecision(1) << p->getWinRate() * 100.0f << "%";
            DrawTextEx(AdwaitaMono, rss.str().c_str(), {padding + nameW + gamesW + winsW, renderPosY}, 20, 0, col);

            unsigned int totalTime = 0;
            const Match *matches = p->getMatches();
            for (int j = 0; j < p->getMatchesNumber(); ++j) totalTime += matches[j].getTimePlayed().count();
            DrawTextEx(AdwaitaMono, (std::to_string(totalTime / 1000) + "s").c_str(), {padding + nameW + gamesW + winsW + rateW, renderPosY}, 20, 0, col);

            renderPosY += 25.0f;
            if (renderPosY > GetScreenHeight() - 100) break;
        }
    }

    Rectangle backButton = {(float)screenWidth / 2.0f - 100.0f, (float)GetScreenHeight() - 60.0f, 200.0f, 40.0f};
    DrawRectangleRec(backButton, DARKGRAY);
    DrawTextEx(AdwaitaMono, "BACK", {backButton.x + 100.0f - MeasureTextEx(AdwaitaMono, "BACK", 24, 0).x / 2.0f, backButton.y + 8.0f}, 24, 0, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), backButton)) DrawRectangleLinesEx(backButton, 2, WHITE);
}

void Render::HandleLeaderboardInput()
{
    if (IsKeyPressed(KEY_ESCAPE)) guiState = GUIState::MENU;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Rectangle backButton = {(float)GetScreenWidth() / 2.0f - 100.0f, (float)GetScreenHeight() - 60.0f, 200.0f, 40.0f};
        if (CheckCollisionPointRec(GetMousePosition(), backButton)) guiState = GUIState::MENU;
    }
}

void Render::SaveMatchStats()
{
    if (statsSaved) return;

    bool won = (game.getCurrentState() == Game::GameState::WON);
    const Map& m = game.getMap();
    Match match(m.getWidth(), m.getHeight(), m.getSeed(), m.getBombs(), game.getCellsRevealed(), game.getClicks(), std::chrono::duration<int, std::milli>(finalTime), won, true);

    if (playerNameText[0] != '\0') {
        PlayerStats *player = Leaderboard::addPlayer(playerNameText);
        if (player) {
            *player + match;
            Leaderboard::addGame();
            statsSaved = true;
        }
    }
}
