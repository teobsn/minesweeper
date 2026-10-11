// Minesweeper headers
#include "cell.hpp"

// C++ standard library
#include <iostream>

Cell::Cell() : bomb(false), flagged(false), revealed(false), adjacentBombs(0), symbol(' ')
{
}

Cell::Cell(bool bomb) : bomb(bomb), flagged(false), revealed(false), adjacentBombs(0), symbol(' ')
{
}

Cell::Cell(bool bomb, bool revealed, short unsigned int adjacentBombs)
    : bomb(bomb), flagged(false), revealed(revealed), adjacentBombs(adjacentBombs), symbol(' ')
{
}

Cell::Cell(bool bomb, bool flagged, bool revealed, short unsigned int adjacentBombs)
    : bomb(bomb), flagged(flagged), revealed(revealed), adjacentBombs(adjacentBombs), symbol(' ')
{
}

Cell::Cell(bool bomb, bool flagged, bool revealed, short unsigned int adjacentBombs, char symbol)
    : bomb(bomb), flagged(flagged), revealed(revealed), adjacentBombs(adjacentBombs), symbol(symbol)
{
}

Cell::Cell(const Cell &other) : IObject(other), bomb(other.bomb), flagged(other.flagged), revealed(other.revealed), adjacentBombs(other.adjacentBombs), symbol(other.symbol)
{
}

Cell::~Cell()
{
#ifdef DEBUG
    std::cout << "\n~Cell()\n";
#endif
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

void Cell::setFlagged(bool value)
{
    flagged = value;
}

char Cell::getSymbol() const
{
    return symbol;
}

Cell &Cell::operator=(const Cell &other)
{
    this->bomb = other.bomb;
    this->adjacentBombs = other.adjacentBombs;
    this->revealed = other.revealed;
    this->flagged = other.flagged;
    this->symbol = other.symbol;

    return *this;
}

bool Cell::operator==(const Cell &other) const
{
    if (this == &other)
        return true;

    return this->bomb == other.bomb &&
           this->revealed == other.revealed &&
           this->flagged == other.flagged &&
           this->adjacentBombs == other.adjacentBombs &&
           this->symbol == other.symbol;
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

void Cell::updateSymbol()
{
    if (bomb)
        symbol = 'B';
    else if (adjacentBombs)
        symbol = static_cast<char>('0' + adjacentBombs);
    else
        symbol = ' ';
}

std::ostream &operator<<(std::ostream &os, const Cell &cell)
{
    return os << cell.toString();
}

std::string Cell::toString() const
{
    if (revealed)
    {
        if (bomb)
            return "B";
        else if (adjacentBombs)
            return std::to_string(adjacentBombs);
        else
            return " ";
    }
    if (flagged)
        return "P";
    return "■";
}

std::istream &operator>>(std::istream &is, Cell &cell)
{
    bool bomb = false;
    bool revealed = false;
    bool flagged = false;
    int adjacentBombs = 0;

    is >> bomb >> revealed >> flagged >> adjacentBombs;

    cell.setBomb(bomb);
    cell.setRevealed(revealed);
    cell.setFlagged(flagged);
    cell.setAdjacentBombs(adjacentBombs);

    cell.updateSymbol();

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

int Cell::operator[](char c) const
{
    switch (c)
    {
    case 'b':
    case 0:
        return this->bomb;
        break;
    case 'a':
    case 1:
        return this->adjacentBombs;
        break;
    case 'f':
    case 2:
        return this->flagged;
        break;
    case 'r':
    case 3:
        return this->revealed;
        break;
    default:
        return 0;
        break;
    }
}
