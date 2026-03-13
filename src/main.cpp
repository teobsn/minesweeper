#include <iostream>

#include "map.hpp"

using namespace std;

int main()
{
    Map map;

    Map map2 = map;

    Map map3(16, 16, 1, 32);

    while(true)
    {
        cin >> map;
        cout << map;
    }

    return 0;
}
