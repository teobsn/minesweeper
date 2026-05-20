#pragma once

#include "templates.hpp"

struct GameSetup
{
    unsigned int width;
    unsigned int height;
    unsigned int bombs;
    unsigned long long seed;

    GameSetup() : width(8), height(8), bombs(8), seed(0) {}

    GameSetup(unsigned int w, unsigned int h, unsigned int b, unsigned long long s)
        : width(w), height(h), bombs(b), seed(s) {}
};

// builder pentru setup-ul de joc; il folosim cand pornim o partida noua
// din meniul cli/gui, ca sa tinem configuratia explicita si curata
class GameSetupBuilder final
{
private:
    GameSetup setup;

public:
    GameSetupBuilder() = default;

    GameSetupBuilder &setWidth(unsigned int value)
    {
        setup.width = value;
        return *this;
    }

    GameSetupBuilder &setHeight(unsigned int value)
    {
        setup.height = value;
        return *this;
    }

    GameSetupBuilder &setBombs(unsigned int value)
    {
        setup.bombs = value;
        return *this;
    }

    GameSetupBuilder &setSeed(unsigned long long value)
    {
        setup.seed = value;
        return *this;
    }

    GameSetup build() const
    {
        GameSetup result = setup;
        // clamp-uim valorile ca sa nu ajungem cu un setup invalid
        result.width = clampValue(result.width, 1u, 128u);
        result.height = clampValue(result.height, 1u, 128u);

        unsigned int maxBombs = result.width * result.height;
        result.bombs = clampValue(result.bombs, 0u, maxBombs);

        return result;
    }
};
