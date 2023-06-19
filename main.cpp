// TO-DO: post game stats, better bomb generation, fix neighbor revealing, better console refresh

// OS Dependent
#ifdef _WIN32
#define OS "WINDOWS"
#include <windows.h>
#include <conio.h>
#define osDependentStart SetConsoleOutputCP(CP_UTF8) // windows terminal doesn't display characters in UTF-8 format by default, this fixes that (it otherwise displays invalid characters)
#endif

#ifdef linux
#define OS "LINUX"
#include <ncurses.h>
#define osDependentStart std::cout << "Linux OS detected\n"
#endif

#ifdef __ANDROID__ // https://stackoverflow.com/questions/6374523/how-to-detect-compilation-by-android-ndk-in-a-c-c-file
#define OS "ANDROID"
#include <conio.h> // android c++ compilers can use conio.h (?)
#define osDependentStart std::cout << "Android detected\n"
#endif

// C++ Libraries
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <thread>
#include <string>
#include <queue>
#include <utility>

// Game headers
#include "customfunctions.h"
#include "controls.h"
#include "matrixneighborconsts.h"

#define br cout << "\n------------------------------------\n"

std::string displayMap[101][101];
bool bombMap[101][101];
int gameMap[101][101] = {0};
int numberMap[101][101] = {0};

int sqSize, bombAmount;

struct cursorStruct
{
    int x;
    int y;
    bool moved;
} cursor;

// Input auxiliary
bool InputUp = false;
bool InputLeft = false;
bool InputDown = false;
bool InputRight = false;
bool InputMark = false;
bool InputHit = false;

// Game loop auxiliary
bool running = true;
bool won = false;

void populateMap()
{
    // populate bomb map

    // https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, sqSize);

    int p1, p2;
    for (int i = 1; i <= bombAmount; i++)
    {
        do // very inefficient
        {
            p1 = distr(gen);
            p2 = distr(gen);
        } while (bombMap[p1][p2]);
        bombMap[p1][p2] = true;
    }

    /* Replaced with curly brackets {0}
    // populate game map
    for (int i = 1; i <= sqSize; i++)
        for (int j = 1; j <= sqSize; j++)
            gameMap[i][j] = 0;

    // populate number map
    for (int i = 1; i <= sqSize; i++)
        for (int j = 1; j <= sqSize; j++)
            numberMap[i][j] = 0;
    */

    for (int i = 1; i <= sqSize; i++)
    {
        for (int j = 1; j <= sqSize; j++)
        {
            if (bombMap[i][j])
            {
                numberMap[i - 1][j - 1]++;
                numberMap[i - 1][j]++;
                numberMap[i - 1][j + 1]++;

                numberMap[i][j - 1]++;
                numberMap[i][j]++; // not theoretically needed
                numberMap[i][j + 1]++;

                numberMap[i + 1][j - 1]++;
                numberMap[i + 1][j]++;
                numberMap[i + 1][j + 1]++;
            }
        }
    }

    // move cursor to center
    cursor.x = sqSize / 2;
    cursor.y = sqSize / 2;
}

void waitForControls()
{
    InputUp = false;
    InputLeft = false;
    InputDown = false;
    InputRight = false;
    InputMark = false;
    InputHit = false;

    #ifdef linux
    initscr();
    cbreak();
    #endif
    int key = getch();
    #ifdef linux
    endwin();
    #endif

    switch (key)
    {
    case KeybindUp:
        InputUp = true;
        break;
    case KeybindLeft:
        InputLeft = true;
        break;
    case KeybindDown:
        InputDown = true;
        break;
    case KeybindRight:
        InputRight = true;
        break;
    case KeybindMark:
        InputMark = true;
        break;
    case KeybindHit:
        InputHit = true;
        break;
    default:
        break;
    }
}

void processCursor()
{
    if (InputUp)
    {
        cursor.y--;
        cursor.moved = true;
    }
    if (InputDown)
    {
        cursor.y++;
        cursor.moved = true;
    }
    if (InputLeft)
    {
        cursor.x--;
        cursor.moved = true;
    }
    if (InputRight)
    {
        cursor.x++;
        cursor.moved = true;
    }

    cursor.x = clamp(cursor.x, 1, sqSize);
    cursor.y = clamp(cursor.y, 1, sqSize);
}

void revealAllBombs()
{
    for (int i = 1; i <= sqSize; i++)
        for (int j = 1; j <= sqSize; j++)
            if (bombMap[i][j])
                gameMap[i][j] = 3;
}

/*
void discoverAdjacentEmpty(int x, int y) // for some reason does not work correctly
{
    if(!bombMap[y + 1][x] && (numberMap[y + 1][x] == 0 || numberMap[y + 1][x] == 1)  && (y + 1 < sqSize + 1) && gameMap[y + 1][x] != 2)
    {
        gameMap[y + 1][x] = 2;
        discoverAdjacentEmpty(y + 1, x);
    }

    if(!bombMap[y - 1][x] && (numberMap[y - 1][x] == 0 || numberMap[y - 1][x] == 1) && (y - 1 > 0) && gameMap[y - 1][x] != 2)
    {
        gameMap[y - 1][x] = 2;
        discoverAdjacentEmpty(y - 1, x);
    }

    if(!bombMap[y][x + 1] && (numberMap[y][x + 1] == 0 || numberMap[y][x + 1] == 1) && (x + 1 < sqSize + 1) && gameMap[y][x + 1] != 2)
    {
        gameMap[y][x + 1] = 2;
        discoverAdjacentEmpty(y, x + 1);
    }

    if(!bombMap[y][x - 1] && (numberMap[y][x - 1] == 0 || numberMap[y][x - 1] == 1) && (x - 1 > 0) && gameMap[y][x - 1] != 2)
    {
        gameMap[y][x - 1] = 2;
        discoverAdjacentEmpty(y, x - 1);
    }
}
*/

void revealNeighbors(int istart, int jstart)
{
    std::queue<std::pair<int, int>> Q;
    Q.push(std::make_pair(istart, jstart));
    // Marking starting coordinates
    gameMap[istart][jstart] = 2;
    while (!Q.empty())
    {
        int i = Q.front().first, j = Q.front().second; // Queue front element coordinates
        for (int k = 0; k < 4; k++)
        {
            int iv = i + di[k], jv = j + dj[k];                                                 // Neighboring coordinates
            if (iv >= 1 && iv <= sqSize && jv >= 1 && jv <= sqSize                              // Is it inside the map?
                && !bombMap[iv][jv] && (numberMap[iv][jv] == 0 || numberMap[iv][jv] == 1)       // Are there no bombs? -- Needs to be changed, real minesweeper reveals differently
                && gameMap[iv][jv] == 0)                                                        // Is the square not revealed?
            {
                // Mark neighboring element
                gameMap[iv][jv] = 2;
                // Add to queue
                Q.push(std::make_pair(iv, jv));
            }
        }
        Q.pop(); // Remove current element from queue
    }
}

bool checkWinByReveal() // checks if all non-bomb squares are revealed
{
    for (int i = 1; i <= sqSize; i++)
        for (int j = 1; j <= sqSize; j++)
            if (gameMap[i][j] != 2 && !bombMap[i][j])
                return false;
    return true;
}

bool checkWinByMarking() // checks if all bombs are marked
{
    for (int i = 1; i <= sqSize; i++)
        for (int j = 1; j <= sqSize; j++)
            if (bombMap[i][j])
                if (gameMap[i][j] != 1)
                    return false;
    return true;
}

void updateGameMap()
{
    // number legend:
    // 0 - not revealed (block)
    // 1 - marked
    // 2 - revealed, safe
    // 3 - revealed, bomb
    if (InputMark && gameMap[cursor.y][cursor.x] != 2)
        gameMap[cursor.y][cursor.x] = !gameMap[cursor.y][cursor.x];

    if (InputHit)
    {
        if (bombMap[cursor.y][cursor.x])
        {
            running = false;
            gameMap[cursor.y][cursor.x] = 3;
            revealAllBombs();
        }
        else
        {
            if (gameMap[cursor.y][cursor.x] == 2)
                cursor.moved = true;
            gameMap[cursor.y][cursor.x] = 2;
            if (doRevealNeighbors)
                revealNeighbors(cursor.y, cursor.x), cursor.moved = true;
        }
    }

    if (
	(checkWinByReveal() && winByRevealing)
	 || (checkWinByMarking() && winByMarking)
    )
    {
        revealAllBombs();
        won = true;
        running = false;
    }
}

void updateDisplayMap()
{
    for (int i = 1; i <= sqSize; i++)
    {
        for (int j = 1; j <= sqSize; j++)
        {
            switch (gameMap[i][j])
            {
            case 0:
                displayMap[i][j] = "█";
                break;
            case 1:
                displayMap[i][j] = "M"; //old: ☒
                break;
            case 2:
                if (numberMap[i][j])
                    displayMap[i][j] = std::to_string(numberMap[i][j]);
                else
                    displayMap[i][j] = "O"; //old: ☐
                break;
            case 3:
                displayMap[i][j] = "B"; // old: ♥
                break;
            default:
                displayMap[i][j] = "?";
                break;
            }
        }
    }

    if (cursor.moved)
    {
        displayMap[cursor.y][cursor.x] = "✗";
        cursor.moved = false;
    }
}

void refreshConsole() // to-do: create string stream with full output and display that (instead of incrementally displaying each square separately)
                      //        in order to reduce number of console refreshes, as code execution waits for the console to finish refreshing
{
    if (OS == "WINDOWS")
        std::cout << "\033c";
    else
        std::cout << u8"\033[2J\033[1;1H"; // https://stackoverflow.com/questions/6486289/how-can-i-clear-console
    std::cout << "\n ";
    for (int i = 1; i <= sqSize; i++)
    {
        for (int j = 1; j <= sqSize; j++)
            std::cout << displayMap[i][j];
        std::cout << "\n ";
    }
    std::cout << "\n ";
}

int gameLoop()
{
    while (running)
    {
        waitForControls();
        processCursor();
        updateGameMap();
        updateDisplayMap();
        refreshConsole();
    }
    return 0;
}

int main()
{
    osDependentStart;
    std::cout << "C++ MINESWEEPER";
    std::br;

    std::cout << "\nSquare size [min 3, max 100]: ";
    std::cin >> sqSize;

    std::cout << "Number of bombs [min 1, max (square size ^ 2)]: ";
    std::cin >> bombAmount;
    std::br;
    std::cout << "Press any key to start!";
    std::br;

    populateMap();
    cursor.moved = true; // fix in order to display cursor before first update
    gameLoop();

    if (won)
        std::cout << "\nCongratulations! You won!\n";
    else
        std::cout << "\nYou lost! Game over!\n";

    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
