// Minesweeper headers
#include "map.hpp"
#include "cell.hpp"

// C++ standard library
#include <vector>
#include <iostream>

// External libraries
#include "pcg/pcg32.h"
#include "pcg/pcg32_8.h"

Map::Map() : width(1), height(1), seed(0), bombs(0)
{
    this->bombs = 0;

    reserveCells();
    // generateBombs(0);
    updateAdjacentBombs(false);
}

Map::Map(const unsigned int width, const unsigned int height, const unsigned long long seed)
    : width(width), height(height), seed(seed)
{
    this->bombs = 0;

    reserveCells();
    generateBombs((width * height) >> 3); // 1 / 8 din totalul celulelor vor fi bombe
    updateAdjacentBombs(false);
}

Map::Map(const unsigned int width, const unsigned int height) : Map(width, height, 0)
{
}

Map::Map(const unsigned int width, const unsigned int height, const unsigned long long seed, unsigned int bombs)
    : width(width), height(height), seed(seed)
{
    this->bombs = 0;
    if (bombs > width * height)
        bombs = width * height;

    reserveCells();
    generateBombs(bombs);
    updateAdjacentBombs(false);
}

Map::Map(const Map &other)
    : width(other.width), height(other.height), seed(other.seed), bombs(other.bombs)
{
    // Cream o matrice noua
    cells = std::vector<std::vector<Cell>>(other.height, std::vector<Cell>(other.width));

    // Copiem celulele
    for (unsigned int y = 0; y < other.height; ++y)
        for (unsigned int x = 0; x < other.width; ++x)
            cells[y][x] = other.cells[y][x];
}

Map::~Map()
{
    std::cout << "\n~Map()\n";
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

    // Cream o matrice noua
    cells = std::vector<std::vector<Cell>>(other.height, std::vector<Cell>(other.width));

    // Copiem celulele
    for (unsigned int y = 0; y < other.height; ++y)
        for (unsigned int x = 0; x < other.width; ++x)
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

bool Map::operator>(const Map &other) const
{
    if (width != other.width || height != other.height)
        return width * height < other.width * other.height;

    return bombs > other.bombs;
}

bool Map::operator<(const Map &other) const
{
    return other > *this;
}

bool Map::operator>=(const Map &other) const
{
    return !(*this < other);
}

bool Map::operator<=(const Map &other) const
{
    return !(*this > other);
}

Map &Map::operator+(unsigned int newBombs)
{
    generateBombs(newBombs);
    updateAdjacentBombs(true);
    return *this;
}

Map &Map::operator*(double factor)
{
    unsigned int newBombs = bombs * factor;

    if (newBombs > width * height)
        newBombs = width * height;

    unsigned int bombDiff = newBombs - bombs;

    if (bombDiff > 0)
        generateBombs(bombDiff);
    else if (bombDiff < 0)
        generateBombs(-bombDiff);

    updateAdjacentBombs(true);
    return *this;
}

Map &operator+(int value, Map &map)
{
    return map + value;
}

Map &operator*(double factor, Map &map)
{
    return map * factor;
}

Map &Map::operator++()
{
    generateBombs(1);
    updateAdjacentBombs(true);

    return *this;
}

Cell Map::operator[](int index)
{
    int x = index % width, y = index / width;

    return this->getCell(x, y);
}

std::ostream &operator<<(std::ostream &os, const Map &map)
{
    // Clear screen
    os << "\033[2J\033[1;1H";

    std::string defaultColor = "\e[0;97m"; // White
    std::string bombColor = "\e[41m";      // Red Background

    std::string colorEscape[] = {
        "\e[0;30m", // 0 -> Black
        "\e[0;94m", // 1 -> Blue
        "\e[0;92m", // 2 -> Green
        "\e[0;91m", // 3 -> Red
        "\e[0;35m", // 4 -> Purple
        "\e[0;31m", // 5 -> Dark Red
        "\e[0;96m", // 6 -> Cyan
        "\e[0;32m", // 7 -> Dark Green
        "\e[0;93m"  // 8 -> Yellow
    };

    for (unsigned int y = 0; y < map.height; ++y)
    {
        for (unsigned int x = 0; x < map.width; ++x)
        {
            if (map.isRevealedAt(x, y))
            {
                if (map.isBombAt(x, y))
                    os << bombColor << "*" << defaultColor << " ";
                else
                {
                    unsigned int adjacentBombs = map.getAdjacentBombsAt(x, y);
                    if (adjacentBombs)
                        os << colorEscape[adjacentBombs] << adjacentBombs << defaultColor << " ";
                    else // Daca nu are bombe adiacente, afisam un spatiu gol
                        os << defaultColor << "  ";
                }
            }
            else
            {
                if (map.isFlaggedAt(x, y))
                    os << defaultColor << "P" << defaultColor << " ";
                else
                    os << defaultColor << "■" << defaultColor << " ";
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

void Map::generateBombs(const int bombs, const bool updateAdjacentBombs)
{
    if (bombs == 0)
        return;

    if (this->bombs + bombs > width * height)
    {
        std::cerr << "Nu se pot genera mai multe bombe decat celulele disponibile! (" << this->bombs + bombs << " > " << width * height << ")";
        return;
    }
    else if (this->bombs + bombs < 0)
    {
        std::cerr << "Nu se pot sterge mai putin bombe decat exista deja! (" << this->bombs + bombs << " < 0)";
        return;
    }

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
            this->bombs++;
            ++i;

            if (updateAdjacentBombs)
                updateAdjacentBombsAround(x, y);
        }
    }

    if (bombs < 0)
    {
        for (unsigned int y = 0; y < height; ++y)
            for (unsigned int x = 0; x < width && i < -bombs; ++x)
                if (cells[y][x].isBomb())
                {
                    cells[y][x].setBomb(false);
                    this->bombs--;
                    i++;

                    updateAdjacentBombsAround(x, y);
                }
    }
}

unsigned int Map::getWidth() const
{
    return width;
}

unsigned int Map::getHeight() const
{
    return height;
}

unsigned int Map::getBombs() const
{
    return bombs;
}

bool Map::isValidPosition(const int x, const int y) const
{
    return x >= 0 && y >= 0 &&
           // Folosim static_cast pentru a evita compararea intre tipuri semnate si nesemnate
           // static_cast e mai explicit decat typecastul clasic din C
           x < static_cast<int>(width) &&
           y < static_cast<int>(height);
}

unsigned int Map::getSeed() const
{
    return seed;
}

bool Map::isBombAt(const int x, const int y) const
{
    return cells[y][x].isBomb();
}

int Map::getAdjacentBombsAt(const int x, const int y) const
{
    return cells[y][x].getAdjacentBombs();
}

bool Map::isRevealedAt(const int x, const int y) const
{
    return cells[y][x].isRevealed();
}

bool Map::isFlaggedAt(const int x, const int y) const
{
    return cells[y][x].isFlagged();
}

void Map::updateAdjacentBombs(bool reset)
{
    if (reset)
        for (unsigned int y = 0; y < height; ++y)
            for (unsigned int x = 0; x < width; ++x)
                cells[y][x].setAdjacentBombs(0);

    // // Pentru restul celulelor, din interior, nu trebuie sa verificam daca pozitia e valida
    // for (unsigned int y = 1; y < height - 1; ++y)
    //     for (unsigned int x = 1; x < width - 1; ++x)
    //         if (cells[y][x].isBomb())
    //         {
    //             // Incrementam numarul de bombe adiacente pentru celulele din jur
    //             for (int dy = -1; dy <= 1; ++dy)
    //                 for (int dx = -1; dx <= 1; ++dx)
    //                 {
    //                     int px = x + dx, py = y + dy;

    //                     if (!cells[py][px].isBomb()) // Bombele nu au nevoie de indicator
    //                         ++cells[py][px];
    //                 }
    //         }

    // // Pentru a nu face mereu verificarea isValidPosition,
    // // calculam separat pentru liniile si coloanele marginale
    // for (unsigned int x = 0; x < width; ++x)
    //     updateAdjacentBombs(x, 0);
    // for (unsigned int x = 0; x < width; ++x)
    //     updateAdjacentBombs(x, height - 1);
    // for (unsigned int y = 1; y < height - 1; ++y)
    //     updateAdjacentBombs(0, y);
    // for (unsigned int y = 1; y < height - 1; ++y)
    //     updateAdjacentBombs(width - 1, y);

    for (unsigned int y = 0; y < height; ++y)
        for (unsigned int x = 0; x < width; ++x)
            if (cells[y][x].isBomb())
            {
                // Incrementam numarul de bombe adiacente pentru celulele din jur
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        int px = x + dx, py = y + dy;

                        if (isValidPosition(px, py))
                            if (!cells[py][px].isBomb()) // Bombele nu au nevoie de indicator
                                ++cells[py][px];
                    }
            }
}

void Map::updateAdjacentBombs(const int x, const int y)
{
    // Actualizam numarul de bombe adiacente pentru celula aflata la x, y
    cells[y][x].setAdjacentBombs(0);
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
        { // Incrementam pentru fiecare vecin care e bomba
            int px = x + dx, py = y + dy;

            if (isValidPosition(px, py) && cells[py][px].isBomb())
                ++cells[y][x];
        }
}

void Map::updateAdjacentBombsAround(const int x, const int y)
{
    // Actualizam numarul de bombe adiacente pentru celulele din jurul celulei aflata la x, y

    int updateRangeX1 = std::max(x - 1, 0), updateRangeX2 = std::min(x + 1, static_cast<int>(width) - 1);
    int updateRangeY1 = std::max(y - 1, 0), updateRangeY2 = std::min(y + 1, static_cast<int>(height) - 1);

    for (int py = updateRangeY1; py <= updateRangeY2; ++py)
        for (int px = updateRangeX1; px <= updateRangeX2; ++px)
            if (!cells[py][px].isBomb()) // Bombele nu au nevoie de indicator
                updateAdjacentBombs(px, py);
}

void Map::reserveCells()
{
    // Rezervam spatiu in memorie pentru celule
    // pentru a evita realocarea ulterioara
    cells.resize(height, std::vector<Cell>(width));
}

Cell &Map::getCell(const int x, const int y)
{
    return cells[y][x];
}

void Map::toggleFlagAt(const int x, const int y)
{
    cells[y][x].toggleFlagged();
}

void Map::setFlagAt(const int x, const int y, const bool value)
{
    cells[y][x].setFlagged(value);
}

void Map::revealAt(const int x, const int y)
{
    cells[y][x].setRevealed(true);
}

void Map::revealAll()
{
    for (unsigned int y = 0; y < height; ++y)
        for (unsigned int x = 0; x < width; ++x)
            cells[y][x].setRevealed(true);
}