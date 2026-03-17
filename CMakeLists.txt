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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/lib)

# Source files
set(SOURCES-CLI
    src/main.cpp
    src/cell.cpp
    src/game.cpp
    src/map.cpp
    src/cli.cpp
)

set(SOURCES-GUI
    src/main-gui.cpp
    src/cell.cpp
    src/game.cpp
    src/map.cpp
    src/render.cpp
)

add_executable(minesweeper ${SOURCES-CLI})
add_executable(minesweeper-gui ${SOURCES-GUI})
add_executable(minesweeper-animation-demo ${SOURCES-CLI})

# Enable parallel builds
set(CMAKE_BUILD_PARALLEL_LEVEL 8)