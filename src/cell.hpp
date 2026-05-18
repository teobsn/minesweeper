#pragma once

// C++ standard library
#include <iostream>
#include <string>

#include "iobject.hpp"

class Cell : public IObject
{
    bool bomb;
    bool flagged;
    bool revealed;
    short unsigned int adjacentBombs;

    char symbol;

public:
    // Constructori
    Cell();
    Cell(bool bomb);
    Cell(bool bomb, bool revealed, short unsigned int adjacentBombs);
    Cell(bool bomb, bool flagged, bool revealed, short unsigned int adjacentBombs);
    Cell(bool bomb, bool flagged, bool revealed, short unsigned int adjacentBombs, char symbol);
    Cell(const Cell &other);
    ~Cell();

    // Operatori atribuire
    Cell &operator=(const Cell &other);

    // Operatori comparatie
    bool operator==(const Cell &other) const;
    bool operator!=(const Cell &other) const;
    bool operator<(const Cell &other) const;
    bool operator>(const Cell &other) const;
    bool operator<=(const Cell &other) const;
    bool operator>=(const Cell &other) const;

    // Operatori aritmetici
    Cell &operator+(const Cell &other);
    Cell &operator-(const Cell &other);
    Cell &operator++();
    Cell &operator--();
    Cell operator++(int);
    Cell operator--(int);
    int operator[](char c) const;

    std::string toString() const override;

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

    bool isFlagged() const;
    void toggleFlagged();
    void setFlagged(bool value);

    char getSymbol() const;
    void updateSymbol();
};
