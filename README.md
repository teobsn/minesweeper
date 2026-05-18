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
- abstract factory for runtime theme selection
- bonus utilities:
  - `Rezultat<T>`
  - `StaticQueue<T, Capacity>`
  - `std::initializer_list<T>` support

## Justificari

### Singleton

- `ThemeManager` este un singleton Meyers fiindca tema activa e stare partajata si trebuie sa aiba un singur punct de adevar in GUI.
- pastrarea lui ca singleton evita sa plimbam starea temei prin cod UI care nu are legatura directa cu ea.

### Abstract Factory

- sistemul de teme are nevoie de schimbare la runtime intre familii de culori si stiluri inrudite.
- abstract factory lasa GUI-ul sa lucreze doar cu interfete, fara sa cunoasca tipurile concrete de teme.

### `Rezultat<T>`

- `Rezultat<T>` e folosit pentru fail-uri de input asteptate, unde un exception complet ar fi prea mult.
- pastreaza explicit path-ul de succes si path-ul de eroare in parsing-ul din CLI.

### `StaticQueue<T, Capacity>`

- queue-ul cu capacitate fixa e util pentru istorii mici si limitate, fara alocare dinamica.

### Bonus patterns

- `Rezultat<T>` ramane pentru tratarea explicita a succesului si erorii in parsing-ul de input.
- `std::initializer_list<T>` face initializarea cu acolade (cand colectia e relativ mica si "fixa").
- parametrii non-tip sunt folositi cand dimensiunea colectiei trebuie impusa la compile time.

## Build

Linux / macOS:
```bash
./build.sh
```

Windows:
```batch
build.bat
```

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

The project is built with aggressive optimization flags for maximum performance:

- **-O3** - Aggressive optimization level
- **-march=native** - CPU-specific instructions (AVX2, SSE4, etc.)
- **-flto** - Link-Time Optimization for whole-program optimization
- **PCG with AVX2** - SIMD-accelerated random number generation