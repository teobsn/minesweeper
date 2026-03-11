#pragma once

class Cell
{
    bool bomb;
    bool revealed;
    unsigned int adjacentBombs;

public:
    Cell();
    Cell(bool bomb);
    Cell(bool bomb, bool revealed, unsigned int adjacentBombs);

    bool isBomb() const;

    void setBomb(bool value);

    bool isRevealed() const;

    void setRevealed(bool value);

    unsigned int getAdjacentBombs() const;

    void setAdjacentBombs(unsigned int value);
};