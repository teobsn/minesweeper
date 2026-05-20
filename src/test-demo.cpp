#include <iostream>
#include <chrono>
#include "cell.hpp"
#include "map.hpp"
#include "stats.hpp"
#include "game.hpp"

using namespace std;

void demoCell()
{
    cout << "\n------ Demo Cell ------\n";

    Cell c1;
    Cell c2(true);
    Cell c3(true, true, 2);
    Cell c4(true, true, true, 6);
    Cell c5(true, true, true, 5, 'B');
    Cell c6(c5);

    cout << c1 << endl;
    cout << c2 << endl;
    cout << c3 << endl;
    cout << c4 << endl;
    cout << c5 << endl;
    cout << c6 << endl;

    c1 = c2;
    cout << c1 << endl;

    c1 = c5;
    cout << c1 << endl;

    cout << (c1 == c2) << endl;
    cout << (c1 != c2) << endl;
    cout << (c3 < c4) << endl;
    cout << (c3 > c4) << endl;
    cout << (c3 <= c4) << endl;
    cout << (c3 >= c4) << endl;

    Cell c7 = c3 + c4;
    cout << c7 << endl;

    Cell c8 = c3 - c4;
    cout << c8 << endl;

    Cell c9 = ++c3;
    cout << c9 << endl;

    Cell c10 = c3++;
    cout << c10 << endl;

    Cell c11 = --c3;
    cout << c11 << endl;

    Cell c12 = c3--;
    cout << c12 << endl;

    cout << c5['b'] << endl;
    cout << c5['a'] << endl;
    cout << c5['f'] << endl;
    cout << c5['r'] << endl;
    cout << c5['?'] << endl;
}

void demoMap()
{
    cout << "\n------ Demo Map ------\n";

    Map map;
    map.revealAll();
    cout << map << endl;

    Map map2(10, 10);
    map2.revealAll();
    cout << map2 << endl;

    Map map3(10, 10, 103232);
    map3.revealAll();
    cout << map3 << endl;

    Map map4(10, 10, 103232, 10);
    map4.revealAll();
    cout << map4 << endl;

    Map map5(map4);
    map5.revealAll();
    cout << map5 << endl;

    Map map6(16, 16, 3333, 40);
    map6 = map5;
    cout << map6 << endl;

    cout << (map5 == map6) << endl;
    cout << (map5 != map6) << endl;
    cout << (map5 < map6) << endl;
    cout << (map5 > map6) << endl;
    cout << (map5 <= map6) << endl;
    cout << (map5 >= map6) << endl;

    Map map7(10, 10, 103232, 5);
    map7.revealAll();
    cout << map7 << endl;

    cout << (map5 == map7) << endl;
    cout << (map5 != map7) << endl;
    cout << (map5 < map7) << endl;
    cout << (map5 > map7) << endl;
    cout << (map5 <= map7) << endl;
    cout << (map5 >= map7) << endl;

    Map map8(10, 10, 103232, 10);
    map8.revealAll();
    cout << map8 << endl;

    ++map8;
    cout << map8 << endl;
    map8 = map8 + 2;
    cout << map8 << endl;
    map8 = 2 + map8;
    cout << map8 << endl;

    map8 = map8 * 2;
    cout << map8 << endl;
    map8 = 1.43 * map8;
    cout << map8 << endl;

    cout << map8[5];
    cout << map8['?'];
}

void demoMatch()
{
    cout << "\n------ Demo Match ------\n";

    Map map1(10, 10, 0, 10);
    Map map2(32, 32, 414, 64);
    Map map3(32, 32, 323, 32);

    map1.revealAt(0, 0);
    map1.revealAt(0, 1);

    Match match1(map1.getWidth(), map1.getHeight(), map1.getSeed());
    match1.setRevealedCells(2);
    match1.setClicks(2);
    // match1.setFinished(true);
    // match1.setWinState(true);
    // match1.setTimePlayed(std::chrono::duration<int, std::milli>(1000000));
    cout << match1 << endl;

    Match match2(map2.getWidth(), map2.getHeight(), map2.getSeed(), map2.getBombs());
    cout << match2 << endl;

    Match match3(map3.getWidth(), map3.getHeight(), map3.getSeed(), map3.getBombs(), 7, 5, std::chrono::duration<int, std::milli>(0), true, true);
    cout << match3 << endl;

    Match match4(match3);
    cout << match4 << endl;

    match4 = match2;
    cout << match4 << endl;

    cout << (match2 == match4) << endl;
    cout << (match2 != match4) << endl;
    cout << (match2 < match4) << endl;
    cout << (match2 > match4) << endl;
    cout << (match2 <= match4) << endl;
    cout << (match2 >= match4) << endl;

    match2 = match2 + 2;
    cout << match2 << endl;
    match2 = 2 + match2;
    cout << match2 << endl;

    match2 = match2 - 2;
    cout << match2 << endl;

    cout << match2['w'] << endl;
    cout << match2['r'] << endl;
    cout << match2['t'] << endl;
    cout << match2['f'] << endl;
    cout << match2['c'] << endl;
    cout << match2['?'] << endl;
}

void demoPlayerStats()
{
    cout << "\n------ Demo Player Stats ------\n";

    PlayerStats playerstat1;
    cout << playerstat1 << endl;

    PlayerStats playerstat2("Player1");
    cout << playerstat2 << endl;

    PlayerStats playerstat3(playerstat2);
    cout << playerstat3 << endl;

    playerstat3 = playerstat2;
    cout << playerstat3 << endl;

    PlayerStats playerstat4("Player2", 0, 0);
    cout << playerstat4 << endl;

    Match match1(32, 32, 0, 64, 65, 11, std::chrono::duration<int, std::milli>(4500000), true, true);
    Match match2(32, 32, 1, 72, 48, 6, std::chrono::duration<int, std::milli>(1000000), false, true);
    Match match3(32, 32, 1, 68, 48, 8, std::chrono::duration<int, std::milli>(1250000), false, true);
    // cout << match1 << endl;

    playerstat4 = playerstat4 + match1;
    playerstat4 = playerstat4 + match2;

    cout << playerstat4 << endl;
    cout << playerstat4 + match3 << endl;

    cout << playerstat4['n'] << endl;
    cout << playerstat4['w'] << endl;
    cout << playerstat4['r'] << endl;
    cout << playerstat4['?'] << endl;

    playerstat3 = playerstat4;
    --playerstat3;
    cout << playerstat3 << endl;

    playerstat3 = playerstat4;
    playerstat3 = playerstat3 - 2;
    cout << playerstat3 << endl;
}

int main()
{
    demoCell();
    demoMap();
    demoMatch();
    demoPlayerStats();

    return 0;
}
