
// Minesweeper headers
#include "game.hpp"
#include "cell.hpp"
#include "map.hpp"
#include "stats.hpp"
#include "render.hpp"
#include "powerups.hpp"
#include "theme_manager.hpp"
#include "exceptions.hpp"

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

namespace
{
const ThemePalette &Theme()
{
    return ThemeManager::instance().getPalette();
}

Color ButtonText(Color background)
{
    int luminance = (299 * background.r + 587 * background.g + 114 * background.b) / 1000;
    return luminance > 140 ? Theme().accentText : Theme().text;
}
}

Render::Render()
{
    strcpy(mapWidthText, "12");
    strcpy(mapHeightText, "12");
    strcpy(mapBombsText, "8");
    strcpy(mapSeedText, "");
    playerName = "Player";
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
    // Informatiile sunt logate pentru debug
    std::cout << "Generating map: " << width << "x" << height << ", Bombs: " << bombs << ", Seed: " << seed << "\n";

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
            ClearBackground(Theme().background);
            DrawMenu();
            EndDrawing();
            break;
        case GUIState::GAME:
            HandleGameInput();
            BeginDrawing();
            ClearBackground(Theme().background);
            DrawGame();
            EndDrawing();
            break;
        case GUIState::MY_STATS:
            HandleMyStatsInput();
            BeginDrawing();
            ClearBackground(Theme().background);
            DrawMyStats();
            EndDrawing();
            break;
        case GUIState::LEADERBOARD:
            HandleLeaderboardInput();
            BeginDrawing();
            ClearBackground(Theme().background);
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
    const ThemePalette &theme = Theme();
    Vector2 mousePos = GetMousePosition();

    const Color textColor = theme.text;
    const Color rectColor = theme.surface;

    float renderPosX = padding, renderPosY = padding, offsetX = screenWidth / 2.0f;
    float sizeX = screenWidth / 2.0f - 2.0f * padding;

    DrawTextEx(AdwaitaMono, "MINESWEEPER", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "MINESWEEPER", 48, 0).x / 2.0f, renderPosY}, 48, 0, theme.accent);
    renderPosY += 80.0f;

    DrawTextEx(AdwaitaMono, "Player Name", {renderPosX, renderPosY}, textSize, 0, textColor);
    Rectangle textBoxPlayerName = {offsetX, renderPosY, sizeX, rectSize};
    DrawRectangleRec(textBoxPlayerName, rectColor);
    DrawTextEx(AdwaitaMono, playerName.c_str(), {textBoxPlayerName.x + 5, textBoxPlayerName.y + 5}, textSize, 0, textColor);

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
        DrawTextEx(AdwaitaMono, "(optional)", {textBoxMapSeed.x + 5, textBoxMapSeed.y + 5}, textSize, 0, theme.muted);
    } else {
        DrawTextEx(AdwaitaMono, mapSeedText, {textBoxMapSeed.x + 5, textBoxMapSeed.y + 5}, textSize, 0, textColor);
    }

    renderPosY += 80.0f;
    Rectangle startButton = {screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    DrawRectangleRec(startButton, theme.success);
    DrawTextEx(AdwaitaMono, "START GAME", {startButton.x + 150.0f - MeasureTextEx(AdwaitaMono, "START GAME", textSize, 0).x / 2.0f, startButton.y + 10.0f}, textSize, 0, ButtonText(theme.success));

    renderPosY += 70.0f;
    Rectangle myStatsButton = {screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    DrawRectangleRec(myStatsButton, theme.accent);
    DrawTextEx(AdwaitaMono, "MY STATISTICS", {myStatsButton.x + 150.0f - MeasureTextEx(AdwaitaMono, "MY STATISTICS", textSize, 0).x / 2.0f, myStatsButton.y + 10.0f}, textSize, 0, ButtonText(theme.accent));

    renderPosY += 70.0f;
    Rectangle leaderboardButton = {screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    DrawRectangleRec(leaderboardButton, theme.highlight);
    DrawTextEx(AdwaitaMono, "LEADERBOARD", {leaderboardButton.x + 150.0f - MeasureTextEx(AdwaitaMono, "LEADERBOARD", textSize, 0).x / 2.0f, leaderboardButton.y + 10.0f}, textSize, 0, ButtonText(theme.highlight));

    renderPosY += 70.0f;
    Rectangle exitButton = {screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    DrawRectangleRec(exitButton, theme.danger);
    DrawTextEx(AdwaitaMono, "EXIT", {exitButton.x + 150.0f - MeasureTextEx(AdwaitaMono, "EXIT", textSize, 0).x / 2.0f, exitButton.y + 10.0f}, textSize, 0, ButtonText(theme.danger));

    renderPosY += 95.0f;
    DrawTextEx(AdwaitaMono, "THEME", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "THEME", 24, 0).x / 2.0f, renderPosY}, 24, 0, theme.text);
    renderPosY += 40.0f;
    std::string currentThemeStr = "Current theme: " + ThemeManager::instance().getThemeName();
    DrawTextEx(AdwaitaMono, currentThemeStr.c_str(), {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, currentThemeStr.c_str(), 18, 0).x / 2.0f, renderPosY - 22.0f}, 18, 0, theme.muted);

    const float themeButtonW = 145.0f;
    const float themeButtonH = 36.0f;
    const float themeLeft = screenWidth / 2.0f - 150.0f;
    const float themeRight = themeLeft + 155.0f;

    Rectangle tokyoNightButton = {themeLeft, renderPosY, themeButtonW, themeButtonH};
    Rectangle oledBlackButton = {themeRight, renderPosY, themeButtonW, themeButtonH};
    renderPosY += 46.0f;
    Rectangle adwaitaButton = {themeLeft, renderPosY, themeButtonW, themeButtonH};
    Rectangle monokaiButton = {themeRight, renderPosY, themeButtonW, themeButtonH};

    auto drawThemeButton = [&](const Rectangle &button, const char *label, ThemeId themeId)
    {
        const bool selected = ThemeManager::instance().getTheme() == themeId;
        DrawRectangleRec(button, selected ? theme.surfaceAlt : theme.surface);
        DrawTextEx(AdwaitaMono, label, {button.x + button.width / 2.0f - MeasureTextEx(AdwaitaMono, label, 18, 0).x / 2.0f, button.y + 8.0f}, 18.0f, 0, selected ? ButtonText(theme.surfaceAlt) : ButtonText(theme.surface));
        if (CheckCollisionPointRec(mousePos, button)) DrawRectangleLinesEx(button, 2, theme.accent);
    };

    drawThemeButton(tokyoNightButton, "TOKYO NIGHT", ThemeId::TokyoNight);
    drawThemeButton(oledBlackButton, "OLED BLACK", ThemeId::OledBlack);
    drawThemeButton(adwaitaButton, "ADWAITA", ThemeId::Adwaita);
    drawThemeButton(monokaiButton, "MONOKAI", ThemeId::Monokai);

    if (CheckCollisionPointRec(mousePos, textBoxPlayerName)) DrawRectangleLinesEx(textBoxPlayerName, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, textBoxMapWidth)) DrawRectangleLinesEx(textBoxMapWidth, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, textBoxMapHeight)) DrawRectangleLinesEx(textBoxMapHeight, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, textBoxMapBombs)) DrawRectangleLinesEx(textBoxMapBombs, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, textBoxMapSeed)) DrawRectangleLinesEx(textBoxMapSeed, 2, YELLOW);
    if (CheckCollisionPointRec(mousePos, startButton)) DrawRectangleLinesEx(startButton, 2, WHITE);
    if (CheckCollisionPointRec(mousePos, myStatsButton)) DrawRectangleLinesEx(myStatsButton, 2, WHITE);
    if (CheckCollisionPointRec(mousePos, leaderboardButton)) DrawRectangleLinesEx(leaderboardButton, 2, WHITE);
    if (CheckCollisionPointRec(mousePos, exitButton)) DrawRectangleLinesEx(exitButton, 2, theme.text);
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
    renderPosY += 70.0f;
    Rectangle exitButton = {(float)screenWidth / 2.0f - 150.0f, renderPosY, 300.0f, 50.0f};
    renderPosY += 95.0f;
    renderPosY += 40.0f;
    const float themeButtonW = 145.0f;
    const float themeButtonH = 36.0f;
    const float themeLeft = screenWidth / 2.0f - 150.0f;
    const float themeRight = themeLeft + 155.0f;
    Rectangle tokyoNightButton = {themeLeft, renderPosY, themeButtonW, themeButtonH};
    Rectangle oledBlackButton = {themeRight, renderPosY, themeButtonW, themeButtonH};
    renderPosY += 46.0f;
    Rectangle adwaitaButton = {themeLeft, renderPosY, themeButtonW, themeButtonH};
    Rectangle monokaiButton = {themeRight, renderPosY, themeButtonW, themeButtonH};

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
        char buffer[64];
        strncpy(buffer, playerName.c_str(), 63);
        buffer[63] = '\0';
        handleTextInput(buffer, 63, false);
        playerName = buffer;
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
    } else if (CheckCollisionPointRec(mousePos, exitButton)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            CloseWindow();
            std::exit(0);
        }
    } else if (CheckCollisionPointRec(mousePos, tokyoNightButton)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ThemeManager::instance().setTheme(makeThemeFactory(ThemeId::TokyoNight));
    } else if (CheckCollisionPointRec(mousePos, oledBlackButton)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ThemeManager::instance().setTheme(makeThemeFactory(ThemeId::OledBlack));
    } else if (CheckCollisionPointRec(mousePos, adwaitaButton)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ThemeManager::instance().setTheme(makeThemeFactory(ThemeId::Adwaita));
    } else if (CheckCollisionPointRec(mousePos, monokaiButton)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ThemeManager::instance().setTheme(makeThemeFactory(ThemeId::Monokai));
    } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void Render::DrawGame()
{
    // Capture reference to avoid copies
    const Map& map = game.getMap();
    const ThemePalette &theme = Theme();
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
    DrawRectangle(0, 0, screenWidth, (int)statsHeight, theme.surface);
    
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
        DrawTextEx(AdwaitaMono, "SHIELD ACTIVE", {(float)screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "SHIELD ACTIVE", 20, 0).x / 2.0f, 45.0f}, 20.0f, 0, theme.warning);
    }

    if (game.getCurrentState() == Game::GameState::WON) {
        DrawTextEx(AdwaitaMono, "YOU WON!", {(float)screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "YOU WON!", 30, 0).x / 2.0f, 15.0f}, 30.0f, 0, theme.success);
    } else if (game.getCurrentState() == Game::GameState::LOST) {
        DrawTextEx(AdwaitaMono, "GAME OVER", {(float)screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "GAME OVER", 30, 0).x / 2.0f, 15.0f}, 30.0f, 0, theme.danger);
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
                DrawRectangleRec(cellRect, theme.surface);
                if (map.isBombAt(x, y)) {
                    DrawRectangleRec(cellRect, theme.danger);
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
                DrawRectangleRec(cellRect, theme.surfaceAlt);
                if (map.isFlaggedAt(x, y)) DrawTexturePro(flagTexture, {0, 0, (float)flagTexture.width, (float)flagTexture.height}, cellRect, {0, 0}, 0, WHITE);
            }
            DrawRectangleLinesEx(cellRect, 1, theme.grid);
        }
    }
    // Interface buttons
    Vector2 mousePos = GetMousePosition();
    
    // Left side buttons: +, *0.75, *1.5
    Rectangle addBombButton = {20.0f, (float)screenHeight - 40.0f, 40.0f, 30.0f};
    DrawRectangleRec(addBombButton, theme.surface);
    DrawTextEx(AdwaitaMono, "+", {addBombButton.x + 20.0f - MeasureTextEx(AdwaitaMono, "+", 20, 0).x / 2.0f, addBombButton.y + 5.0f}, 20.0f, 0, ButtonText(theme.surface));
    if (CheckCollisionPointRec(mousePos, addBombButton)) DrawRectangleLinesEx(addBombButton, 2, theme.accent);

    Rectangle scaleDownButton = {65.0f, (float)screenHeight - 40.0f, 60.0f, 30.0f};
    DrawRectangleRec(scaleDownButton, theme.surface);
    DrawTextEx(AdwaitaMono, "*0.75", {scaleDownButton.x + 30.0f - MeasureTextEx(AdwaitaMono, "*0.75", 20, 0).x / 2.0f, scaleDownButton.y + 5.0f}, 20.0f, 0, ButtonText(theme.surface));
    if (CheckCollisionPointRec(mousePos, scaleDownButton)) DrawRectangleLinesEx(scaleDownButton, 2, theme.accent);

    Rectangle scaleBombsButton = {130.0f, (float)screenHeight - 40.0f, 60.0f, 30.0f};
    DrawRectangleRec(scaleBombsButton, theme.surface);
    DrawTextEx(AdwaitaMono, "*1.5", {scaleBombsButton.x + 30.0f - MeasureTextEx(AdwaitaMono, "*1.5", 20, 0).x / 2.0f, scaleBombsButton.y + 5.0f}, 20.0f, 0, ButtonText(theme.surface));
    if (CheckCollisionPointRec(mousePos, scaleBombsButton)) DrawRectangleLinesEx(scaleBombsButton, 2, theme.accent);

    // Power-up buttons
    Rectangle shieldBtn = {200.0f, (float)screenHeight - 40.0f, 80.0f, 30.0f};
    DrawRectangleRec(shieldBtn, theme.accent);
    DrawTextEx(AdwaitaMono, "SHIELD", {shieldBtn.x + 40.0f - MeasureTextEx(AdwaitaMono, "SHIELD", 18, 0).x / 2.0f, shieldBtn.y + 6.0f}, 18.0f, 0, ButtonText(theme.accent));
    if (CheckCollisionPointRec(mousePos, shieldBtn)) {
        DrawRectangleLinesEx(shieldBtn, 2, theme.warning);
        BasePowerUp* temp = new ShieldPowerUp(0, 0);
        DrawTextEx(AdwaitaMono, temp->getDescription().c_str(), {shieldBtn.x, shieldBtn.y - 25.0f}, 16, 0, theme.warning);
        delete temp;
    }

    Rectangle ultiBtn = {290.0f, (float)screenHeight - 40.0f, 80.0f, 30.0f};
    DrawRectangleRec(ultiBtn, theme.highlight);
    DrawTextEx(AdwaitaMono, "ULTI", {ultiBtn.x + 40.0f - MeasureTextEx(AdwaitaMono, "ULTI", 18, 0).x / 2.0f, ultiBtn.y + 6.0f}, 18.0f, 0, ButtonText(theme.highlight));
    if (CheckCollisionPointRec(mousePos, ultiBtn)) {
        DrawRectangleLinesEx(ultiBtn, 2, theme.accent);
        BasePowerUp* temp = new UltimatePowerUp(0, 0, 0);
        DrawTextEx(AdwaitaMono, temp->getDescription().c_str(), {ultiBtn.x, ultiBtn.y - 25.0f}, 16, 0, theme.accent);
        delete temp;
    }

    Rectangle defuseBtn = {380.0f, (float)screenHeight - 40.0f, 80.0f, 30.0f};
    DrawRectangleRec(defuseBtn, theme.surfaceAlt);
    DrawTextEx(AdwaitaMono, "DEFUSE", {defuseBtn.x + 40.0f - MeasureTextEx(AdwaitaMono, "DEFUSE", 18, 0).x / 2.0f, defuseBtn.y + 6.0f}, 18.0f, 0, ButtonText(theme.surfaceAlt));
    if (CheckCollisionPointRec(mousePos, defuseBtn)) {
        DrawRectangleLinesEx(defuseBtn, 2, theme.text);
        BasePowerUp* temp = new BombDefuser(0, 0);
        DrawTextEx(AdwaitaMono, temp->getDescription().c_str(), {defuseBtn.x, defuseBtn.y - 25.0f}, 16, 0, theme.text);
        delete temp;
    }

    // Right side button: MENU
    Rectangle backButton = {(float)screenWidth - 220.0f, (float)screenHeight - 40.0f, 200.0f, 30.0f};
    DrawRectangleRec(backButton, theme.surface);
    DrawTextEx(AdwaitaMono, "MENU", {backButton.x + 100.0f - MeasureTextEx(AdwaitaMono, "MENU", 20, 0).x / 2.0f, backButton.y + 5.0f}, 20.0f, 0, ButtonText(theme.surface));
    if (CheckCollisionPointRec(mousePos, backButton)) DrawRectangleLinesEx(backButton, 2, theme.accent);
}

void Render::HandleGameInput()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    Vector2 mousePos = GetMousePosition();
    static double lastRevealClickTime = -1.0;
    static int lastRevealClickX = -1;
    static int lastRevealClickY = -1;
    // using constexpr here instead of a macro, since c++11 gives us a proper compile-time constant
    constexpr double doubleClickThreshold = 0.30;

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

    try {
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
                return;
            }
            if (CheckCollisionPointRec(mousePos, ultiBtn)) {
                BasePowerUp* p = new UltimatePowerUp(3, 10.0f, 5);
                p->use(game, mouseGridX, mouseGridY);
                return;
            }
            if (CheckCollisionPointRec(mousePos, defuseBtn)) {
                BasePowerUp* p = new BombDefuser(4, 0.5);
                p->use(game, mouseGridX, mouseGridY);
                return;
            }
        }

        if (mousePos.x >= startX && mousePos.x < startX + (float)width * cellSize && mousePos.y >= startY && mousePos.y < startY + (float)height * cellSize) {
            if (game.getCurrentState() == Game::GameState::IN_PROGRESS || game.getCurrentState() == Game::GameState::NOT_STARTED) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    bool clickedCellIsRevealed = game.getMap().isRevealedAt(mouseGridX, mouseGridY);
                    double now = GetTime();
                    if (clickedCellIsRevealed) {
                        if (lastRevealClickX == mouseGridX && lastRevealClickY == mouseGridY && (now - lastRevealClickTime) <= doubleClickThreshold) {
                            game.adjacentReveal(mouseGridX, mouseGridY);
                            lastRevealClickTime = -1.0;
                            lastRevealClickX = -1;
                            lastRevealClickY = -1;
                        } else {
                            lastRevealClickTime = now;
                            lastRevealClickX = mouseGridX;
                            lastRevealClickY = mouseGridY;
                        }
                        return;
                    }

                    lastRevealClickTime = -1.0;
                    lastRevealClickX = -1;
                    lastRevealClickY = -1;

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
        }
        if (IsKeyPressed(KEY_TWO)) {
            BasePowerUp* p = new ScannerPowerUp(2, 3);
            p->use(game, mouseGridX, mouseGridY);
        }
        if (IsKeyPressed(KEY_THREE)) {
            BasePowerUp* p = new UltimatePowerUp(3, 10.0f, 5);
            p->use(game, mouseGridX, mouseGridY);
        }
        if (IsKeyPressed(KEY_FOUR)) {
            BasePowerUp* p = new BombDefuser(4, 0.5);
            p->use(game, mouseGridX, mouseGridY);
        }
    }
    catch (const InvalidMoveException &e) {
        std::cout << e.what() << "\n";
    }
    catch (const GameConfigException &e) {
        std::cout << e.what() << "\n";
    }
    catch (const DatabaseException &e) {
        std::cout << e.what() << "\n";
    }
    catch (const MinesweeperException &e) {
        std::cout << e.what() << "\n";
    }
    catch (...) {
        std::cout << "An unknown error occurred.\n";
    }
}

void Render::DrawMyStats()
{
    const int screenWidth = GetScreenWidth();
    const float padding = 40.0f;
    const ThemePalette &theme = Theme();
    float renderPosY = padding;

    DrawTextEx(AdwaitaMono, "MY STATISTICS", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "MY STATISTICS", 48, 0).x / 2.0f, renderPosY}, 48, 0, theme.accent);
    renderPosY += 70.0f;

    PlayerStats *player = Leaderboard::findPlayer(playerName);
    if (!player || player->getMatchesNumber() == 0) {
        DrawTextEx(AdwaitaMono, "No games found for this player.", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "No games found for this player.", 24, 0).x / 2.0f, renderPosY + 100.0f}, 24, 0, theme.text);
    } else {
        // Two-column layout for general stats
        float colWidth = (screenWidth - 2.0f * padding) / 2.0f;
        
        // Column 1
        std::stringstream ss1;
        ss1 << "Player: " << player->getPlayerName() << "\n";
        ss1 << "Total Games: " << player->getMatchesNumber() << "\n";
        ss1 << "Win Rate: " << std::fixed << std::setprecision(1) << player->getWinRate() * 100.0f << "%";
        DrawTextEx(AdwaitaMono, ss1.str().c_str(), {padding, renderPosY}, 24, 0, theme.text);

        // Column 2
        std::stringstream ss2;
        ss2 << "Wins: " << player->getWins() << "\n";
        ss2 << "Losses: " << (player->getMatchesNumber() - player->getWins()) << "\n";
        
        unsigned int totalTime = 0;
        unsigned int totalClicks = 0;
        const auto &matches = player->getMatches();
        int totalGames = player->getMatchesNumber();
        for (const Match &match : matches) {
            totalTime += match.getTimePlayed().count();
            totalClicks += match.getClicks();
        }
        ss2 << "Total Time: " << (totalTime / 1000) << "s\n";
        ss2 << "Avg Clicks: " << (totalGames > 0 ? totalClicks / totalGames : 0);
        DrawTextEx(AdwaitaMono, ss2.str().c_str(), {padding + colWidth, renderPosY}, 24, 0, theme.text);

        renderPosY += 140.0f;

        // Recent Games Table
        DrawTextEx(AdwaitaMono, "RECENT GAMES", {padding, renderPosY}, 28, 0, theme.highlight);
        renderPosY += 40.0f;

        // Table Legend
        float resW = 70, sizeW = 90, bombsW = 70, timeW = 70, clickW = 70;
        DrawTextEx(AdwaitaMono, "Result", {padding, renderPosY}, 20, 0, theme.muted);
        DrawTextEx(AdwaitaMono, "Size", {padding + resW, renderPosY}, 20, 0, theme.muted);
        DrawTextEx(AdwaitaMono, "Bombs", {padding + resW + sizeW, renderPosY}, 20, 0, theme.muted);
        DrawTextEx(AdwaitaMono, "Time", {padding + resW + sizeW + bombsW, renderPosY}, 20, 0, theme.muted);
        DrawTextEx(AdwaitaMono, "Clicks", {padding + resW + sizeW + bombsW + timeW, renderPosY}, 20, 0, theme.muted);
        DrawTextEx(AdwaitaMono, "Powers", {padding + resW + sizeW + bombsW + timeW + clickW, renderPosY}, 20, 0, theme.muted);
        
        renderPosY += 30.0f;
        DrawRectangle(padding, renderPosY - 5, screenWidth - 2 * padding, 2, theme.grid);

        int start = std::max(0, totalGames - 12);
        for (int i = totalGames - 1; i >= start; --i) {
            Color resColor = matches[i].getWinState() ? theme.success : theme.danger;
            DrawTextEx(AdwaitaMono, matches[i].getWinState() ? "WIN" : "LOSS", {padding, renderPosY}, 18, 0, resColor);
            
            std::string sizeStr = std::to_string(matches[i].getMapWidth()) + "x" + std::to_string(matches[i].getMapHeight());
            DrawTextEx(AdwaitaMono, sizeStr.c_str(), {padding + resW, renderPosY}, 18, 0, WHITE);
            DrawTextEx(AdwaitaMono, std::to_string(matches[i].getMapBombs()).c_str(), {padding + resW + sizeW, renderPosY}, 18, 0, WHITE);
            DrawTextEx(AdwaitaMono, (std::to_string(matches[i].getTimePlayed().count() / 1000) + "s").c_str(), {padding + resW + sizeW + bombsW, renderPosY}, 18, 0, WHITE);
            DrawTextEx(AdwaitaMono, std::to_string(matches[i].getClicks()).c_str(), {padding + resW + sizeW + bombsW + timeW, renderPosY}, 18, 0, WHITE);
            DrawTextEx(AdwaitaMono, std::to_string(matches[i].getTotalPowerUpsUsed()).c_str(), {padding + resW + sizeW + bombsW + timeW + clickW, renderPosY}, 18, 0, theme.highlight);
            
            renderPosY += 22.0f;
        }
    }

    Rectangle removeLastButton = {padding, (float)GetScreenHeight() - 60.0f, 250.0f, 40.0f};
    DrawRectangleRec(removeLastButton, theme.danger);
    DrawTextEx(AdwaitaMono, "REMOVE LAST MATCH", {removeLastButton.x + 125.0f - MeasureTextEx(AdwaitaMono, "REMOVE LAST MATCH", 20, 0).x / 2.0f, removeLastButton.y + 10.0f}, 20.0f, 0, theme.accentText);
    if (CheckCollisionPointRec(GetMousePosition(), removeLastButton)) DrawRectangleLinesEx(removeLastButton, 2, theme.accent);

    Rectangle backButton = {(float)screenWidth - padding - 200.0f, (float)GetScreenHeight() - 60.0f, 200.0f, 40.0f};
    DrawRectangleRec(backButton, theme.surface);
    DrawTextEx(AdwaitaMono, "BACK", {backButton.x + 100.0f - MeasureTextEx(AdwaitaMono, "BACK", 24, 0).x / 2.0f, backButton.y + 8.0f}, 24, 0, theme.text);
    if (CheckCollisionPointRec(GetMousePosition(), backButton)) DrawRectangleLinesEx(backButton, 2, theme.accent);
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
            PlayerStats *player = Leaderboard::findPlayer(playerName);
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
    const float padding = 40.0f;
    const ThemePalette &theme = Theme();
    float renderPosY = padding;

    DrawTextEx(AdwaitaMono, "LEADERBOARD", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "LEADERBOARD", 48, 0).x / 2.0f, renderPosY}, 48, 0, theme.accent);
    renderPosY += 70.0f;

    Leaderboard::sort();
    unsigned int playerCount = Leaderboard::getPlayerCount();
    const auto players = Leaderboard::getPlayers();
    const PlayerStats *bestPlayer = findBest<PlayerStats>(players);

    if (playerCount == 0) {
        DrawTextEx(AdwaitaMono, "No players yet. Play some games first!", {screenWidth / 2.0f - MeasureTextEx(AdwaitaMono, "No players yet. Play some games first!", 24, 0).x / 2.0f, renderPosY + 100.0f}, 24, 0, theme.text);
    } else {
        float colWidth = (screenWidth - 2.0f * padding) / 2.0f;
        
        std::stringstream ss1;
        ss1 << "Players: " << playerCount << "\n";
        ss1 << "Total Games: " << Leaderboard::getOverallGames() << "\n";
        ss1 << "Best Player: " << (bestPlayer ? bestPlayer->getPlayerName() : "Unknown");
        DrawTextEx(AdwaitaMono, ss1.str().c_str(), {padding, renderPosY}, 24, 0, theme.text);

        std::stringstream ss2;
        if (bestPlayer)
            ss2 << "Best Win Rate: " << std::fixed << std::setprecision(1) << bestPlayer->getWinRate() * 100.0f << "%\n";
        else
            ss2 << "Best Win Rate: 0.0%\n";
        ss2 << "Current Player: " << (playerName.empty() ? "None" : playerName) << "\n";
        ss2 << "Sorted: By score";
        DrawTextEx(AdwaitaMono, ss2.str().c_str(), {padding + colWidth, renderPosY}, 24, 0, theme.text);

        renderPosY += 140.0f;

        DrawTextEx(AdwaitaMono, "PLAYER RANKING", {padding, renderPosY}, 28, 0, theme.highlight);
        renderPosY += 40.0f;

        float nameW = 200, gamesW = 80, winsW = 80, rateW = 100;
        DrawTextEx(AdwaitaMono, "Player", {padding, renderPosY}, 20, 0, theme.muted);
        DrawTextEx(AdwaitaMono, "Games", {padding + nameW, renderPosY}, 20, 0, theme.muted);
        DrawTextEx(AdwaitaMono, "Wins", {padding + nameW + gamesW, renderPosY}, 20, 0, theme.muted);
        DrawTextEx(AdwaitaMono, "Win Rate", {padding + nameW + gamesW + winsW, renderPosY}, 20, 0, theme.muted);
        DrawTextEx(AdwaitaMono, "Time", {padding + nameW + gamesW + winsW + rateW, renderPosY}, 20, 0, theme.muted);
        renderPosY += 30.0f;
        DrawRectangle(padding, renderPosY - 5, screenWidth - 2 * padding, 2, theme.grid);

        for (unsigned int i = 0; i < playerCount; ++i) {
            PlayerStats *p = Leaderboard::getPlayer(i);
            bool isCurrent = (p->getPlayerName() == playerName);
            bool isBest = (bestPlayer && p == bestPlayer);
            Color col = isCurrent ? theme.success : (isBest ? theme.warning : theme.text);

            DrawTextEx(AdwaitaMono, p->getPlayerName().c_str(), {padding, renderPosY}, 20, 0, col);
            DrawTextEx(AdwaitaMono, std::to_string(p->getMatchesNumber()).c_str(), {padding + nameW, renderPosY}, 20, 0, col);
            DrawTextEx(AdwaitaMono, std::to_string(p->getWins()).c_str(), {padding + nameW + gamesW, renderPosY}, 20, 0, col);
            
            std::stringstream rss;
            rss << std::fixed << std::setprecision(1) << p->getWinRate() * 100.0f << "%";
            DrawTextEx(AdwaitaMono, rss.str().c_str(), {padding + nameW + gamesW + winsW, renderPosY}, 20, 0, col);

            unsigned int totalTime = 0;
            const auto &matches = p->getMatches();
            for (const Match &match : matches) totalTime += match.getTimePlayed().count();
            DrawTextEx(AdwaitaMono, (std::to_string(totalTime / 1000) + "s").c_str(), {padding + nameW + gamesW + winsW + rateW, renderPosY}, 20, 0, col);

            renderPosY += 25.0f;
            if (renderPosY > GetScreenHeight() - 100) break;
        }
    }

    Rectangle deleteButton = {padding, (float)GetScreenHeight() - 60.0f, 250.0f, 40.0f};
    Rectangle backButton = {(float)screenWidth - padding - 200.0f, (float)GetScreenHeight() - 60.0f, 200.0f, 40.0f};
    DrawRectangleRec(deleteButton, theme.danger);
    DrawTextEx(AdwaitaMono, "DELETE PLAYER", {deleteButton.x + 125.0f - MeasureTextEx(AdwaitaMono, "DELETE PLAYER", 20, 0).x / 2.0f, deleteButton.y + 10.0f}, 20.0f, 0, ButtonText(theme.danger));
    DrawRectangleRec(backButton, theme.surface);
    DrawTextEx(AdwaitaMono, "BACK", {backButton.x + 100.0f - MeasureTextEx(AdwaitaMono, "BACK", 24, 0).x / 2.0f, backButton.y + 8.0f}, 24, 0, theme.text);
    if (CheckCollisionPointRec(GetMousePosition(), deleteButton)) DrawRectangleLinesEx(deleteButton, 2, theme.accent);
    if (CheckCollisionPointRec(GetMousePosition(), backButton)) DrawRectangleLinesEx(backButton, 2, theme.accent);
}

void Render::HandleLeaderboardInput()
{
    if (IsKeyPressed(KEY_ESCAPE)) guiState = GUIState::MENU;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Rectangle deleteButton = {40.0f, (float)GetScreenHeight() - 60.0f, 250.0f, 40.0f};
        Rectangle backButton = {(float)GetScreenWidth() - 40.0f - 200.0f, (float)GetScreenHeight() - 60.0f, 200.0f, 40.0f};
        if (CheckCollisionPointRec(GetMousePosition(), deleteButton)) {
            try
            {
                if (!playerName.empty())
                    Leaderboard::removePlayer(playerName);
            }
            catch (const MinesweeperException &e)
            {
                std::cout << e.what() << "\n";
            }
            catch (...)
            {
                std::cout << "An unknown error occurred.\n";
            }
        }
        if (CheckCollisionPointRec(GetMousePosition(), backButton)) guiState = GUIState::MENU;
    }
}

void Render::SaveMatchStats()
{
    if (statsSaved) return;

    bool won = (game.getCurrentState() == Game::GameState::WON);
    const Map& m = game.getMap();
    Match match(m.getWidth(), m.getHeight(), m.getSeed(), m.getBombs(), game.getCellsRevealed(), game.getClicks(), std::chrono::duration<int, std::milli>(finalTime), won, true);
    
    // Set power-up statistics
    match.setPowerUpStats(
        game.getPowerUpCount("Shield"),
        game.getPowerUpCount("Scanner"),
        game.getPowerUpCount("Ultimate"),
        game.getPowerUpCount("Defuser")
    );

    if (!playerName.empty()) {
        // Functia Raylib SaveFileText cere un pointer non-const.
        SaveFileText("last_player.txt", const_cast<char*>(playerName.c_str()));

        PlayerStats *player = Leaderboard::addPlayer(playerName);
        if (player) {
            *player + match;
            Leaderboard::addGame();
            statsSaved = true;
        }
    }
}
