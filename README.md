# Minesweeper

A C++ implementation of the Minesweeper game with both CLI and GUI interfaces.

## Features

- classic minesweeper gameplay with reveal, flag, chord and double-click chord
- CLI menu and GUI menu with the same game state
- leaderboard with CRUD, sorting and filtering
- recent match history
- multiple GUI themes: `Tokyo Night`, `OLED Black`, `Adwaita`, `Monokai`
- custom exception hierarchy and try/catch based error handling
- template utilities used in real gameplay logic
- STL containers and algorithms
- `std::string` everywhere instead of manual C strings
- `IObject` interface with unique ids and polymorphic `toString()`
- singleton `ThemeManager` implemented as a Meyers singleton
- singleton `Logger` with journal persistence and menu access
- abstract factory for runtime theme selection
- builder for game setup creation
- bonus utilities:
  - `Rezultat<T>`
  - `StaticQueue<T, Capacity>`
  - `std::initializer_list<T>` support

## Justificari / Detalii proiect POO

### Varianta aleasa
- Am ales varianta A, extinderea proiectului 2

### Justificari containere STL

- `std::vector<Map::cells>` este potrivit pentru harta jocului, deoarece accesul la o celula dupa coordonate trebuie sa fie rapid `O(1)`, iar structura ramane compacta in memorie.
- `std::vector<Match>` pastreaza istoricul complet al meciurilor pentru fiecare jucator, fiind util pentru iterare secventiala si agregari peste tot istoricul; `push_back` este amortizat `O(1)`.
- `std::unordered_map<std::string, PlayerStats>` este alegerea naturala pentru leaderboard, deoarece cautarea si inserarea jucatorilor sunt medii `O(1)`.
- `std::vector<std::string>` pentru ordinea leaderboard-ului este util fiindca permite afisare ordonata si sortare eficienta `O(n log n)`.
- `std::list<BasePowerUp*>` este folosita pentru istoricul power-up-urilor activate, deoarece stergerea si curatarea completa sunt `O(1)` pe nod si nu cer realocari sau mutari de elemente.
- `std::queue<std::pair<int, int>>` este potrivita pentru algoritmul BFS de revelare a celulelor, fiind o coada FIFO cu `push`/`pop` amortizate `O(1)`.
- `StaticQueue<Match, 10>` pastreaza ultimele 10 meciuri vizibile in UI, fara alocari dinamice suplimentare; `push`/`pop` sunt `O(1)`, iar dimensiunea este fixa la compilare.

### Justificari design pattern Builder

- `GameSetupBuilder` e folosit cand construim configuratia unui joc nou din input-ul meniului.
- mentine setup-ul explicit si lasa CLI-ul si GUI-ul sa normalizeze acelasi path inainte sa creeze harta.


## Build

### Windows (Fresh Install)

To set up the environment and build on a clean Windows installation, run the following commands in **PowerShell**:

```powershell
# 1. Install Scoop (Package Manager)
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression

# 2. Install Build Tools
scoop install gcc make cmake

# 3. Install Raylib (MinGW version)
scoop bucket add extras
scoop install raylib-mingw

# 4. Build the project
.\build.bat
```

Linux / macOS:
```bash
./build.sh
```

Windows:
```batch
build.bat
```

The scripts above build the project in CMake's `Debug` configuration by default,
which also enables the project's `DEBUG` macro inside the codebase.

To force a release build locally, pass the build type explicitly:
```bash
BUILD_TYPE=Release ./build.sh
```

```batch
build.bat Release
```

The GitHub Actions workflow only builds `Release` binaries.

Build outputs:
- `build/minesweeper` - CLI version
- `build/minesweeper-gui` - GUI version
- `build/minesweeper-benchmark` - benchmark utility
- `build/minesweeper-animation-demo` - map generation animation demo

## Dependencies

- `raylib` - GUI rendering
- `PCG` - random number generation with AVX2 SIMD acceleration
- `CMake` - build system
- `C++23` - language standard

## Build Optimizations

Builds are produced with aggressive optimization flags for maximum performance:

- `-O3` - Aggressive optimization level
- `-march=native` - CPU-specific instructions (AVX2, SSE4, etc.)
- `-flto` - Link-Time Optimization for whole-program optimization
- `PCG with AVX2` - SIMD-accelerated random number generation
