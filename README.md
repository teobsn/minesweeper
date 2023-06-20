# Minesweeper

Console minesweeper app written in C++

Features:
- Color output
- Neighbor revealing

## Compiling

### Linux
Dependencies: `g++` compiler, `ncurses` library
1. Clone the repository:
   - `git clone https://github.com/teobsn/minesweeper.git`
   - `cd minesweeper`
2. Compile using g++:
   - `g++ -o minesweeper main.cpp -lncurses`

The resulting file is `minesweeper`

#### Compiling for Windows within Linux
Dependencies: `g++` compiler, mingw64-g++` compiler
1. Clone the repository:
   - `git clone https://github.com/teobsn/minesweeper.git`
   - `cd minesweeper`
2. Compile using mingw:
   - `x86_64-w64-mingw32-g++ main.cpp -o minesweeper.exe -static`
     
The resulting file is `minesweeper.exe`

### Windows
TODO
