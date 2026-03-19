# Minesweeper

A C++ implementation of the classic Minesweeper game with both CLI and GUI interfaces.

![Animation Demo](./docs/readme/anim1.mkv)

## Features

### Core Gameplay
- **Classic Minesweeper mechanics** - Reveal cells, place flags, avoid bombs
- **Flood fill reveal** - Clicking an empty cell cascades to reveal connected empty areas
- **Chording** - When a numbered cell has correct flags around it, chord to reveal remaining neighbors
- **Seeded RNG** - Reproducible bomb placement using PCG random generator

### Interfaces
- **CLI Version** - Feature-rich terminal interface with ANSI colors
- **GUI Version** - Raylib-based graphical interface
- **Animation Demo** - Visual demonstration of game algorithms
- **Benchmark** - Simple benchmark test consisting of random map generation

## Building

```bash
./build.sh
```

Build outputs:
- `build/minesweeper` - CLI version
- `build/minesweeper-gui` - GUI version  
- `build/minesweeper-benchmark` - Benchmark utilities
- `build/minesweeper-animation-demo` - Animation demo

## Dependencies

- **raylib** - GUI rendering
- **PCG** - Random number generation with AVX2 SIMD acceleration (included)
- **CMake** - Build system
- **C++23** - Language standard

### Technical Highlights

- ANSI color output in terminal
- Benchmark framework for algorithm profiling

### Build Optimizations

The project is built with aggressive optimization flags for maximum performance:

- **Clang** - Modern C++ compiler with excellent optimization
- **-O3** - Aggressive optimization level
- **-march=native** - CPU-specific instructions (AVX2, SSE4, etc.)
- **-flto** - Link-Time Optimization for whole-program optimization
- **PCG with AVX2** - SIMD-accelerated random number generation