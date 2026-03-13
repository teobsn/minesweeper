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

Cell::Cell(bool bomb, bool revealed, unsigned int adjacentBombs)
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

Cell &Cell::operator=(const Cell &other)
{
    this->bomb = other.bomb;
    this->adjacentBombs = other.adjacentBombs;

    // Vrem sa pastram stadiul de afisare
    // this->revealed = other.revealed;

    return *this;
}

bool Cell::operator==(const Cell &other) const
{
    if (this == &other)
        return true;

    // Consideram ca celule sunt egale daca fie ambele sunt bombe sau niciuna nu e bomba
    // Sau daca au acelasi numar de bombe adiacente
    return (this->bomb == other.bomb) || (this->adjacentBombs == other.adjacentBombs);
}

bool Cell::operator!=(const Cell &other) const
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Cell &cell)
{ // de testat
    if (cell.revealed)
    {
        if (cell.bomb)
            os << "B";
        else if (cell.adjacentBombs)
            os << cell.adjacentBombs;
        else
            os << " ";
    }
    else
        os << "O";

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

/*
de implementat:
constructor de atribuire
constructor ++, --, doar facend o copie a celulei, dar cu numarul de bombe adiacente actualizat corespunzator
constructor de +, -, doar facend o copie a celulei, dar cu numarul de bombe adiacente actualizat corespunzator

operator []

operator de >=, verifica daca celula are mai multe bombe adiacente decat cealalta
spaceship?
*/