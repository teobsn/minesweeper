cmake_minimum_required(VERSION 3.10)

project(Minesweeper)

# Use Clang compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Enable LTO
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always -Wall -Wextra -O3 -flto -march=native -lraylib -lGL -lm -lpthread -ldl -lrt -lX11")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Source files
set(SOURCES-CLI
    src/main.cpp
    src/cell.cpp
    src/game.cpp
    src/map.cpp
    src/stats.cpp
    src/cli.cpp
)

set(SOURCES-GUI
    src/main-gui.cpp
    src/cell.cpp
    src/game.cpp
    src/map.cpp
    src/render.cpp
)

set(SOURCES-BENCHMARK
    src/benchmark.cpp
    src/cell.cpp
    src/game.cpp
    src/map.cpp
    src/cli.cpp
)

set(SOURCES-DEMO1
    src/animation-demo.cpp
    src/cell.cpp
    src/game.cpp
    src/map.cpp
    src/cli.cpp
)

set(SOURCES-DEMO2
    src/test-demo.cpp
    src/cell.cpp
    src/game.cpp
    src/map.cpp
    src/cli.cpp
    src/stats.cpp
)


add_executable(minesweeper ${SOURCES-CLI})
add_executable(minesweeper-gui ${SOURCES-GUI})
add_executable(minesweeper-benchmark ${SOURCES-BENCHMARK})
add_executable(minesweeper-animation-demo ${SOURCES-DEMO1})
add_executable(minesweeper-test ${SOURCES-DEMO2})

# Enable parallel builds
set(CMAKE_BUILD_PARALLEL_LEVEL 8)