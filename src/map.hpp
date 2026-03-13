#pragma once

// Minesweeper headers
#include "cell.hpp"

// C++ standard library
#include <vector>
#include <iostream>

class Map
{
    unsigned int width, height;
    unsigned long long seed;
    unsigned int bombs;
    std::vector<std::vector<Cell>> cells;

    void reserveCells();
    void generateBombs(const unsigned int bombs);
    void updateAdjacentBombs();

    bool isValidPosition(int x, int y) const;
    bool isBombAt(int x, int y) const;
    int getAdjacentBombsAt(int x, int y) const;

    Cell &getCell(int x, int y);

public:
    // Constructori
    Map();
    Map(const unsigned int width, const unsigned int height);
    Map(const unsigned int width, const unsigned int height, const unsigned long long seed);
    Map(const unsigned int width, const unsigned int height, const unsigned long long seed, const unsigned int bombs);

    // Copy constructor
    Map(const Map &other);

    // Destructor
    ~Map();

    // Operatori
    Map &operator=(const Map &other);
    bool operator==(const Map &other) const;
    bool operator!=(const Map &other) const;

    // Friend pentru ostream, istream
    friend std::ostream &operator<<(std::ostream &os, const Map &map);
    friend std::istream &operator>>(std::istream &os, Map &map);
};

std::ostream &operator<<(std::ostream &os, const Map &map);