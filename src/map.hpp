#pragma once

// Minesweeper headers
#include "cell.hpp"

// C++ standard library
#include <vector>
#include <iostream>

class Map final
{
    unsigned int width, height;
    unsigned long long seed;
    unsigned int bombs;
    // Matricea de celule este modelata ca vector de vectori deoarece accesul
    // la o celula dupa coordonate ramane O(1), iar rearanjarea dinamica este rara
    std::vector<std::vector<Cell>> cells;

    void reserveCells();
    void generateBombs(const int bombs, const bool updateAdjacentBombs = false);

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

    // Operator de atribuire
    Map &operator=(const Map &other);

    // Operatori de comparatie
    bool operator==(const Map &other) const;
    bool operator!=(const Map &other) const;

    bool operator>=(const Map &other) const;
    bool operator<=(const Map &other) const;
    bool operator>(const Map &other) const;
    bool operator<(const Map &other) const;

    // Operatori aritmetici
    Map &operator+(int bombs);
    Map &operator-(int bombs);
    Map &operator*(double factor);
    Map &operator++();

    // Friend pentru +, -, *
    friend Map &operator+(int value, Map &map);
    friend Map &operator-(int value, Map &map);
    friend Map &operator*(double factor, Map &map);

    // Operator []
    Cell &operator[](int index);

    // Friend pentru ostream, istream
    friend std::ostream &operator<<(std::ostream &os, const Map &map);
    friend std::istream &operator>>(std::istream &os, Map &map);

    // Alte metode
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getBombs() const;
    bool isValidPosition(const int x, const int y) const;

    unsigned long long getSeed() const;
    void setSeed(const unsigned long long newSeed);

    bool isBombAt(const int x, const int y) const;
    int getAdjacentBombsAt(const int x, const int y) const;
    bool isRevealedAt(const int x, const int y) const;
    bool isFlaggedAt(const int x, const int y) const;

    void updateAdjacentBombs(const bool reset = true);        // Actualizeaza toata harta
    void updateAdjacentBombs(const int x, const int y);       // Actualizeaza indicatorul celulei aflate la x, y
    void updateAdjacentBombsAround(const int x, const int y); // Actualizaza indicatorul celulelor din jurul celei aflate la x, y

    void toggleFlagAt(const int x, const int y);
    void setFlagAt(const int x, const int y, const bool value);
    void revealAt(const int x, const int y);

    void revealAll();
    void revealArea(int x, int y, int radius);

    void clearGrid();
};

std::ostream &operator<<(std::ostream &os, const Map &map);
std::istream &operator>>(std::istream &os, Map &map);
