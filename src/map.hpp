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
    void generateBombs(const unsigned int bombs, const bool updateAdjacentBombs = false);
    void updateAdjacentBombs(const bool reset = true);  // Actualizeaza toata harta
    void updateAdjacentBombs(const int x, const int y);       // Actualizeaza indicatorul celulei aflate la x, y
    void updateAdjacentBombsAround(const int x, const int y); // Actualizaza indicatorul celulelor din jurul celei aflate la x, y

    bool isValidPosition(const int x, const int y) const;
    bool isBombAt(const int x, const int y) const;
    int getAdjacentBombsAt(const int x, const int y) const;

    Cell &getCell(const int x, const int y);

public:
    // Constructori
    Map();
    Map(const unsigned int width, const unsigned int height);
    Map(const unsigned int width, const unsigned int height, const unsigned long long seed);
    Map(const unsigned int width, const unsigned int height, const unsigned long long seed, unsigned int bombs);

    // Copy constructor
    Map(const Map &other);

    // Destructor
    ~Map();

    // Operatori de comparatie
    Map &operator=(const Map &other);
    bool operator==(const Map &other) const;
    bool operator!=(const Map &other) const;

    bool operator>=(const Map &other) const;
    bool operator<=(const Map &other) const;
    bool operator>(const Map &other) const;
    bool operator<(const Map &other) const;

    // Operatori aritmetici
    Map &operator+(unsigned int bombs);
    Map &operator*(double factor);
    Map &operator++();

    // Operator []
    Cell operator[](int index);

    // Friend pentru ostream, istream
    friend std::ostream &operator<<(std::ostream &os, const Map &map);
    friend std::istream &operator>>(std::istream &os, Map &map);
};

std::ostream &operator<<(std::ostream &os, const Map &map);
std::istream &operator>>(std::istream &os, Map &map);