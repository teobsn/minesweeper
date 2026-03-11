#pragma once

// Minesweeper library
#include "cell.hpp"

// C++ standard library
#include <vector>
#include <iostream>



class Map
{
    const unsigned int width, height;
    const unsigned long long seed;
    std::vector<std::vector<Cell>> cells;

    void generateBombs(const unsigned int bombs);
    bool isValidPosition(int x, int y) const;
    void updateAdjacentBombs();
    void reserveCells();

    Cell &getCell(int x, int y);

public:
    Map();
    Map(const unsigned int width, const unsigned int height);
    Map(const unsigned int width, const unsigned int height, const unsigned long long seed);
    Map(const unsigned int width, const unsigned int height, const unsigned long long seed, const unsigned int bombs);

    friend std::ostream &operator<<(std::ostream &os, Map &map);

    ~Map();
};

std::ostream &operator<<(std::ostream &os, const Map &map);