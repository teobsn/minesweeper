
// Minesweeper headers
#include "game.hpp"
#include "cell.hpp"
#include "map.hpp"
#include "stats.hpp"
#include "render.hpp"

// External libraries
#include "raylib.h"

// C++ standard library
#include <iostream>
#include <chrono>
#include <cstring>
#include <cstdlib>

Render::Render()
{
    // mapWidthText[5] = "16";
    // mapHeightText[5] = "16";
    // mapBombsText[5] = "32";
    // mapSeedText[13] = "";

    strcpy(mapWidthText, "16");
    strcpy(mapHeightText, "16");
    strcpy(mapBombsText, "32");
    strcpy(mapSeedText, "");

    // this->game = game;
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

    game.setMap(Map(width, height, bombs, seed));

    std::cout << game.getMap();
}

void Render::StartRender()
{
    game = Game(Game::GameState::MENU);

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);

    const int screenWidth = 500;
    const int screenHeight = 600;

    // Initialize window
    InitWindow(screenWidth, screenHeight, "Minesweeper");

    // Load font
    Font AdwaitaSans = LoadFont("resources/fonts/AdwaitaSans-Regular.ttf");
    Font AdwaitaMono = LoadFont("resources/fonts/AdwaitaMono-Regular.ttf");

    while (!WindowShouldClose())
    {
        Game::GameState gameState = game.getCurrentState();
        if (gameState == Game::GameState::MENU)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            const float padding = 16;
            const float textSize = 32;
            const float rectSize = 36;

            const Color textColor = WHITE;
            const Color rectColor = DARKGRAY;

            float renderPosX = padding, renderPosY = padding, offsetX = screenWidth / 2.0f + padding;
            float sizeX = screenWidth / 2.0f - 3 * padding;

            DrawTextEx(AdwaitaMono, "Map Width", {renderPosX, renderPosY}, textSize, 0, textColor);
            Rectangle textBoxMapWidth = {renderPosX + offsetX, renderPosY, sizeX, rectSize};
            DrawRectangleRec(textBoxMapWidth, rectColor);
            DrawTextEx(AdwaitaMono, mapWidthText, {textBoxMapWidth.x + textSize / 16, textBoxMapWidth.y + textSize / 8}, textSize, 0, textColor);

            renderPosY += 48;
            DrawTextEx(AdwaitaMono, "Map Height", {renderPosX, renderPosY}, textSize, 0, textColor);
            Rectangle textBoxMapHeight = {renderPosX + offsetX, renderPosY, sizeX, rectSize};
            DrawRectangleRec(textBoxMapHeight, rectColor);
            DrawTextEx(AdwaitaMono, mapHeightText, {textBoxMapHeight.x + textSize / 16, textBoxMapHeight.y + textSize / 8}, textSize, 0, textColor);

            renderPosY += 48;
            DrawTextEx(AdwaitaMono, "Bombs", {renderPosX, renderPosY}, textSize, 0, textColor);
            Rectangle textBoxMapBombs = {renderPosX + offsetX, renderPosY, sizeX, rectSize};
            DrawRectangleRec(textBoxMapBombs, rectColor);
            DrawTextEx(AdwaitaMono, mapBombsText, {textBoxMapBombs.x + textSize / 16, textBoxMapBombs.y + textSize / 8}, textSize, 0, textColor);

            renderPosY += 48;
            DrawTextEx(AdwaitaMono, "Seed", {renderPosX, renderPosY}, textSize, 0, textColor);
            Rectangle textBoxMapSeed = {renderPosX + offsetX, renderPosY, sizeX, rectSize};
            DrawRectangleRec(textBoxMapSeed, rectColor);
            DrawTextEx(AdwaitaMono, mapSeedText, {textBoxMapSeed.x + textSize / 16, textBoxMapSeed.y + textSize / 8}, textSize, 0, textColor);

            renderPosX = (screenWidth - sizeX) / 2, renderPosY = (screenHeight - rectSize) / 2;
            Rectangle startButton = {renderPosX, renderPosY, sizeX, rectSize};
            DrawRectangleRec(startButton, rectColor);
            DrawTextEx(AdwaitaMono, "Start", {startButton.x + sizeX / 2 - textSize * strlen("Start") / 4, startButton.y + textSize / 8}, textSize, 0, textColor);

            if (CheckCollisionPointRec(GetMousePosition(), textBoxMapWidth))
            {
                DrawRectangleLines((int)textBoxMapWidth.x, (int)textBoxMapWidth.y, (int)textBoxMapWidth.width, (int)textBoxMapWidth.height, textColor);
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();
                while (key > 0)
                {
                    if ((key >= 48) && (key <= 57) && (strlen(mapWidthText) < 4))
                    {
                        int len = strlen(mapWidthText);

                        mapWidthText[len] = (char)key;
                        mapWidthText[len + 1] = '\0'; // Add null terminator at the end of the string
                    }

                    key = GetCharPressed(); // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                    if (strlen(mapWidthText) > 0)
                        mapWidthText[strlen(mapWidthText) - 1] = '\0';
            }
            else if (CheckCollisionPointRec(GetMousePosition(), textBoxMapHeight))
            {
                DrawRectangleLines((int)textBoxMapHeight.x, (int)textBoxMapHeight.y, (int)textBoxMapHeight.width, (int)textBoxMapHeight.height, textColor);
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();
                while (key > 0)
                {
                    if ((key >= 48) && (key <= 57) && (strlen(mapHeightText) < 4))
                    {
                        int len = strlen(mapHeightText);

                        mapHeightText[len] = (char)key;
                        mapHeightText[len + 1] = '\0'; // Add null terminator at the end of the string
                    }

                    key = GetCharPressed(); // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                    if (strlen(mapHeightText) > 0)
                        mapHeightText[strlen(mapHeightText) - 1] = '\0';
            }
            else if (CheckCollisionPointRec(GetMousePosition(), textBoxMapBombs))
            {
                DrawRectangleLines((int)textBoxMapWidth.x, (int)textBoxMapBombs.y, (int)textBoxMapBombs.width, (int)textBoxMapBombs.height, textColor);
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();
                while (key > 0)
                {
                    if ((key >= 48) && (key <= 57) && (strlen(mapBombsText) < 4))
                    {
                        int len = strlen(mapBombsText);

                        mapBombsText[len] = (char)key;
                        mapBombsText[len + 1] = '\0'; // Add null terminator at the end of the string
                    }

                    key = GetCharPressed(); // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                    if (strlen(mapBombsText) > 0)
                        mapBombsText[strlen(mapBombsText) - 1] = '\0';
            }
            else if (CheckCollisionPointRec(GetMousePosition(), textBoxMapSeed))
            {
                DrawRectangleLines((int)textBoxMapWidth.x, (int)textBoxMapSeed.y, (int)textBoxMapSeed.width, (int)textBoxMapSeed.height, textColor);
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();
                while (key > 0)
                {
                    if ((key >= 48) && (key <= 57) && (strlen(mapSeedText) < 12))
                    {
                        int len = strlen(mapSeedText);

                        mapSeedText[len] = (char)key;
                        mapSeedText[len + 1] = '\0'; // Add null terminator at the end of the string
                    }

                    key = GetCharPressed(); // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                    if (strlen(mapSeedText) > 0)
                        mapSeedText[strlen(mapSeedText) - 1] = '\0';
            }
            else if (CheckCollisionPointRec(GetMousePosition(), startButton))
            {
                DrawRectangleLines((int)startButton.x, (int)startButton.y, (int)startButton.width, (int)startButton.height, textColor);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    unsigned int height = atoi(mapHeightText);
                    unsigned int width = atoi(mapWidthText);
                    unsigned int bombs = atoi(mapBombsText);
                    unsigned long long seed;

                    if (strcmp(mapSeedText, ""))
                        seed = atoi(mapHeightText);
                    else
                    {
                        seed = rand();
                        std::cout << "Generating random seed " << seed << "\n";
                    }

                    InitializeGame(width, height, bombs, seed);
                }
            }
            else
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }

            EndDrawing();
        }
    }
    CloseWindow();
}