// Minesweeper headers
#include "cell.hpp"

// C++ standard library
#include <iostream>

Cell::Cell() : bomb(false), revealed(false), adjacentBombs(0)
{
}

Cell::Cell(bool bomb) : bomb(bomb), revealed(false), adjacentBombs(0)
{
}

Cell::Cell(bool bomb, bool revealed, short unsigned int adjacentBombs)
    : bomb(bomb), revealed(revealed), adjacentBombs(adjacentBombs)
{
}

Cell::Cell(const Cell &other) : bomb(other.bomb), revealed(other.revealed), adjacentBombs(other.adjacentBombs)
{
}

Cell::~Cell()
{
}

bool Cell::isBomb() const
{
    return bomb;
}

void Cell::setBomb(bool value)
{
    bomb = value;
}

bool Cell::isRevealed() const
{
    return revealed;
}

void Cell::setRevealed(bool value)
{
    revealed = value;
}

unsigned int Cell::getAdjacentBombs() const
{
    return adjacentBombs;
}

void Cell::setAdjacentBombs(unsigned int value)
{
    adjacentBombs = value;
}

bool Cell::isFlagged() const
{
    return flagged;
}

void Cell::toggleFlagged()
{
    flagged = !flagged;
}

Cell &Cell::operator=(const Cell &other)
{
    this->bomb = other.bomb;
    this->adjacentBombs = other.adjacentBombs;
    this->revealed = other.revealed;
    this->flagged = other.flagged;

    return *this;
}

bool Cell::operator==(const Cell &other) const
{
    if (this == &other)
        return true;

    return this->bomb == other.bomb &&
           this->revealed == other.revealed &&
           this->flagged == other.flagged &&
           this->adjacentBombs == other.adjacentBombs;
}

bool Cell::operator!=(const Cell &other) const
{
    return !(*this == other);
}

bool Cell::operator<(const Cell &other) const
{
    return this->adjacentBombs < other.adjacentBombs;
}

bool Cell::operator>(const Cell &other) const
{
    return other < *this;
}

bool Cell::operator<=(const Cell &other) const
{
    return !(*this > other);
}

bool Cell::operator>=(const Cell &other) const
{
    return !(*this < other);
}

std::ostream &operator<<(std::ostream &os, const Cell &cell)
{
    if (cell.bomb)
        os << "B";
    else if (cell.adjacentBombs)
        os << cell.adjacentBombs;
    else
        os << " ";

    return os;
}

std::istream &operator>>(std::istream &is, Cell &cell)
{ // de testat
    bool bomb = false;
    bool revealed = false;
    int adjacentBombs = 0;

    is >> bomb >> revealed >> adjacentBombs;

    // cell.bomb = bomb;
    // cell.revealed = revealed;
    // cell.adjacentBombs = adjacentBombs;
    cell.setBomb(bomb);
    cell.setRevealed(revealed);
    cell.setAdjacentBombs(adjacentBombs);

    return is;
}

Cell &Cell::operator+(const Cell &other)
{
    adjacentBombs += other.adjacentBombs;
    return *this;
}

Cell &Cell::operator-(const Cell &other)
{
    adjacentBombs -= other.adjacentBombs;
    return *this;
}

Cell &Cell::operator++()
{
    ++adjacentBombs;
    return *this;
}

Cell Cell::operator++(int)
{
    Cell temp = *this;
    ++*this;
    return temp;
}

Cell &Cell::operator--()
{
    --adjacentBombs;
    return *this;
}

Cell Cell::operator--(int)
{
    Cell temp = *this;
    --*this;
    return temp;
}

/*
de implementat:

operator []

*/