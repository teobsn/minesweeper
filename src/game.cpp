
// Minesweeper headers
#include "game.hpp"
#include "cell.hpp"
#include "map.hpp"
#include "stats.hpp"

// External libraries
#include "raylib.h"

// C++ standard library
#include <iostream>
#include <chrono>
#include <cstring>

Game::Game()
{
    gameState = GameState::MENU;
    flagsPlaced = 0;
    cellsRevealed = 0;
    startTime = std::chrono::steady_clock::now();
}

void Game::checkWinCondition()
{
}
void Game::loseGame()
{
}

void Game::startGame()
{
}

void Game::revealCell(int x, int y)
{
}

void Game::toggleFlag(int x, int y)
{
}

Game::GameState Game::getCurrentState() const
{
    return gameState;
}

const Map &Game::getMap() const
{
}

std::chrono::duration<int,std::milli> Game::getElapsedTime() const
{
}
