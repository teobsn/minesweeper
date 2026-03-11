// Minesweeper library
#include "map.hpp"
#include "cell.hpp"

// C++ standard library
#include <vector>
#include <iostream>

// External libraries
#include "lib/pcg32.h"
#include "lib/pcg32_8.h"

void Map::generateBombs(const unsigned int bombs)
{
    // Impartim seed-ul in doua jumatati pentru a le folosi la initializarea pRNG-ului
    unsigned int seed_init = seed >> 32;
    unsigned int seed_seq = seed & ((1ULL << 32) - 1);

    // Initializam pRNG
    pcg32 rng;
    rng.seed(seed_init, seed_seq);

    // Generam pozitiile bombelor
    unsigned int i = 0;
    while (i < bombs)
    {
        unsigned int x = rng.nextUInt(width);
        unsigned int y = rng.nextUInt(height);

        if (!cells[y][x].isBomb())
        {
            cells[y][x].setBomb(true);
            ++i;
        }
    }
}

bool Map::isValidPosition(int x, int y) const
{
    return x >= 0 && y >= 0 && x < static_cast<int>(width) && y < static_cast<int>(height);
}

void Map::updateAdjacentBombs()
{
    for (unsigned int y = 0; y < height; ++y)
        for (unsigned int x = 0; x < width; ++x)
            if (cells[y][x].isBomb())
            {
                // Incrementam numarul de bombe adiacente pentru celulele din jur
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        int px = x + dx, py = y + dy;

                        if (isValidPosition(px, py) && !cells[py][px].isBomb())
                            // ++cells[py][px];
                            cells[py][px].setAdjacentBombs(cells[py][px].getAdjacentBombs() + 1);
                    }
            }
}

void Map::reserveCells()
{
    // Rezervam spatiu in memorie pentru celule
    // pentru a evita realocarea ulterioara
    cells.resize(height);
    for (unsigned int i = 0; i < height; ++i)
        cells[i].resize(width);

    // Initializam celulele
    for (unsigned int y = 0; y < height; ++y)
        for (unsigned int x = 0; x < width; ++x)
            cells[y].emplace_back(Cell(false));
}

Cell &Map::getCell(int x, int y)
{
    return cells[y][x];
}

Map::Map() : width(8), height(8), seed(0)
{
    reserveCells();
    generateBombs((width * height) >> 3);
    updateAdjacentBombs();
}

Map::Map(const unsigned int width, const unsigned int height, const unsigned long long seed)
    : width(width), height(height), seed(seed)
{
    int bombs = (width * height) >> 3; // 1 / 8 din totalul celulelor vor fi bombe

    reserveCells();
    generateBombs(bombs);
    updateAdjacentBombs();
}

Map::Map(const unsigned int width, const unsigned int height) : Map(width, height, 0)
{
}

Map::Map(const unsigned int width, const unsigned int height, const unsigned long long seed, const unsigned int bombs)
    : width(width), height(height), seed(seed)
{
    reserveCells();
    generateBombs(bombs);
    updateAdjacentBombs();
}

/*
de implementat:
copy constructor
constructor de atribuire
constructor de +, *

operator =
operator >>
operator []
operator ==, !=

idee: constructor de + care sa combine doua harti, adica sa puna bombe din ambele harti, iar numarul de bombe adiacente sa fie actualizat corespunzator
idee: constructor de * care sa combine doua harti, adica sa puna bombe doar acolo unde ambele harti au bombe, iar numarul de bombe adiacente sa fie actualizat corespunzator

operator de >=, verifica daca o harta are mai multe bombe decat cealalta
eventual spaceship
*/

Map::~Map()
{
    // e ceva de pus aici? vedem...
}

std::ostream &operator<<(std::ostream &os, Map &map)
{
    for (unsigned int y = 0; y < map.height; ++y)
    {
        for (unsigned int x = 0; x < map.width; ++x)
        {
            if (map.cells[y][x].isBomb())
                os << "* ";
            else
            {
                unsigned int adjacentBombs = map.cells[y][x].getAdjacentBombs();
                os << (adjacentBombs ? adjacentBombs : ' ') << " ";
            }
        }
        os << std::endl;
    }

    return os;
}