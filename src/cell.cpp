#include "cell.hpp"

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

/*
de implementat:
copy constructor
constructor de atribuire
constructor ++, --, doar facend o copie a celulei, dar cu numarul de bombe adiacente actualizat corespunzator
constructor de +, -, doar facend o copie a celulei, dar cu numarul de bombe adiacente actualizat corespunzator

operator =
operator << 
operator >>
operator []
operator ==, !=

operator de >=, verifica daca celula are mai multe bombe adiacente decat cealalta
spaceship?
*/