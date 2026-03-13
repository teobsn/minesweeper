#pragma once

// C++ standard library
#include <iostream>

class Cell
{
    bool bomb;
    bool revealed;
    unsigned int adjacentBombs;

public:
    // Constructori
    Cell();
    Cell(bool bomb);
    Cell(bool bomb, bool revealed, unsigned int adjacentBombs);
    Cell(const Cell &other);
    ~Cell();

    // Operatori
    Cell &operator=(const Cell &other);
    bool operator==(const Cell &other) const;
    bool operator!=(const Cell &other) const;

    // Friend pentru ostream, istream
    friend std::ostream &operator<<(std::ostream &os, const Cell &cell);
    friend std::istream &operator>>(std::istream &os, Cell &cell);

    // Metode
    bool isBomb() const;

    void setBomb(bool value);

    bool isRevealed() const;

    void setRevealed(bool value);

    unsigned int getAdjacentBombs() const;

    void setAdjacentBombs(unsigned int value);
};