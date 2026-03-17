
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


Render::Render(Game game)
{
    // mapWidthText[5] = "16";
    // mapHeightText[5] = "16";
    // mapBombsText[5] = "32";
    // mapSeedText[13] = "";

    strcpy(mapWidthText, "16");
    strcpy(mapHeightText, "16");
    strcpy(mapBombsText, "32");
    strcpy(mapSeedText, "");

    this->game = game;
}

Render::~Render()
{
}

void Render::StartRender()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);

    const int screenWidth = 500;
    const int screenHeight = 600;

    Font AdwaitaSans = LoadFont("resources/fonts/AdwaitaSans-Regular.ttf");
    Font AdwaitaMono = LoadFont("resources/fonts/AdwaitaMono-Regular.ttf");

    // Initialize window
    InitWindow(screenWidth, screenHeight, "Minesweeper");

    // Load font

    Game::GameState gameState = game.getCurrentState();

    while (!WindowShouldClose())
    {
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

            renderPosX = 16, renderPosY = 16;

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
            else
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }

            EndDrawing();
        }
    }
    CloseWindow();
}