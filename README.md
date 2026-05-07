# Minesweeper

A C++ implementation of the classic Minesweeper game with both CLI and GUI interfaces.

## Downloads

Pre-built binaries for **Windows**, **macOS**, and **Linux** are automatically generated for every commit (with GitHub Actions) and can be found in the [GitHub Releases](https://github.com/teobsn/minesweeper-poo/releases) section.

## Building

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
- `build/minesweeper-benchmark` - Benchmark utility
- `build/minesweeper-animation-demo` - Map generation animation demo

## Dependencies

- **raylib** - GUI rendering
- **PCG** - Random number generation with AVX2 SIMD acceleration (included)
- **CMake** - Build system
- **C++23** - Language standard

### Build Optimizations

The project is built with aggressive optimization flags for maximum performance:

- **-O3** - Aggressive optimization level
- **-march=native** - CPU-specific instructions (AVX2, SSE4, etc.)
- **-flto** - Link-Time Optimization for whole-program optimization
- **PCG with AVX2** - SIMD-accelerated random number generation