// Minesweeper headers
#include "map.hpp"
#include "cell.hpp"

// C++ standard library
#include <vector>
#include <iostream>

// External libraries
#include "pcg/pcg32.h"
#include "pcg/pcg32_8.h"

Map::Map() : width(8), height(8), seed(0)
{
    this->bombs = (width * height) >> 3; // 1 / 8 din totalul celulelor vor fi bombe

    reserveCells();
    generateBombs(this->bombs);
    updateAdjacentBombs();
}

Map::Map(const unsigned int width, const unsigned int height, const unsigned long long seed)
    : width(width), height(height), seed(seed)
{
    this->bombs = (width * height) >> 3; // 1 / 8 din totalul celulelor vor fi bombe

    reserveCells();
    generateBombs(this->bombs);
    updateAdjacentBombs();
}

Map::Map(const unsigned int width, const unsigned int height) : Map(width, height, 0)
{
}

Map::Map(const unsigned int width, const unsigned int height, const unsigned long long seed, const unsigned int bombs)
    : width(width), height(height), seed(seed)
{
    this->bombs = bombs;

    reserveCells();
    generateBombs(bombs);
    updateAdjacentBombs();
}

Map::Map(const Map &other)
    : width(other.width), height(other.height), seed(other.seed), bombs(other.bombs)
{
    reserveCells();
    for (unsigned int y = 0; y < height; ++y)
        for (unsigned int x = 0; x < width; ++x)
            cells[y][x] = other.cells[y][x];
}

/*
de implementat:
constructor de +, *

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

Map &Map::operator=(const Map &other)
{
    // Verificam daca incercam sa atribuim aceeasi harta
    if (this == &other)
        return *this;

    // Copiem atributele
    width = other.width;
    height = other.height;
    seed = other.seed;
    bombs = other.bombs;

    // Modificam dimensiunea vectorului de celule daca e necesar
    if (other.height != height)
        cells.resize(height);

    if (other.width != width)
        for (unsigned int i = 0; i < height; ++i)
            cells[i].resize(width);

    // Copiem celulele
    for (unsigned int y = 0; y < height; ++y)
        for (unsigned int x = 0; x < width; ++x)
            cells[y][x] = other.cells[y][x];

    return *this;
}

bool Map::operator==(const Map &other) const
{
    if (this == &other)
        return true;

    if (width != other.width || height != other.height)
        return false;

    for (unsigned int y = 0; y < height; ++y)
        for (unsigned int x = 0; x < width; ++x)
            if (cells[y][x] != other.cells[y][x])
                return false;

    return true;
}

bool Map::operator!=(const Map &other) const
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Map &map)
{
    for (unsigned int y = 0; y < map.height; ++y)
    {
        for (unsigned int x = 0; x < map.width; ++x)
        {
            if (map.isBombAt(x, y))
                os << "* ";
            else
            {
                unsigned int adjacentBombs = map.getAdjacentBombsAt(x, y);
                if (adjacentBombs)
                    os << adjacentBombs << " ";
                else // Daca nu are bombe adiacente, afisam un spatiu gol
                    os << "  ";
            }
        }
        os << std::endl;
    }

    return os;
}

std::istream &operator>>(std::istream &is, Map &map)
{
    unsigned int width, height, bombs;
    unsigned long long seed;

    is >> width >> height >> seed >> bombs;

    Map newMap(width, height, seed, bombs);
    map = newMap;

    return is;
}

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
    return x >= 0 && y >= 0 &&
           // Folosim static_cast pentru a evita compararea intre tipuri semnate si nesemnate
           // static_cast e mai explicit decat typecastul clasic din C
           x < static_cast<int>(width) &&
           y < static_cast<int>(height);
}

bool Map::isBombAt(int x, int y) const
{
    return cells[y][x].isBomb();
}

int Map::getAdjacentBombsAt(int x, int y) const
{
    return cells[y][x].getAdjacentBombs();
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