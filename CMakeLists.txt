cmake_minimum_required(VERSION 3.10)

project(Minesweeper)

# Enable LTO
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
if(WIN32)
    option(BUILD_CLI "Build CLI version" OFF)
else()
    option(BUILD_CLI "Build CLI version" ON)
endif()

# Compiler flags
if(MSVC)
    add_compile_options(/W4 /O2 /permissive-)
else()
    add_compile_options(-Wall -Wextra -O3 -flto)
    if(NOT APPLE)
        add_compile_options(-march=native)
    endif()
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Source groups
set(SOURCES-COMMON
    src/cell.cpp
    src/game.cpp
    src/map.cpp
    src/stats.cpp
)

set(SOURCES-GUI
    ${SOURCES-COMMON}
    src/main-gui.cpp
    src/render.cpp
)

# CLI-related sources
if(BUILD_CLI)
    set(SOURCES-CLI
        ${SOURCES-COMMON}
        src/main.cpp
        src/cli.cpp
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
endif()

# Raylib finding/linking
if(NOT raylib_FOUND)
    find_package(raylib QUIET)
endif()

# GUI Target
add_executable(minesweeper-gui ${SOURCES-GUI})
if(raylib_FOUND)
    target_link_libraries(minesweeper-gui raylib)
else()
    # Fallback to manual linking if find_package fails (e.g. on Linux with system raylib)
    if(APPLE)
        target_link_libraries(minesweeper-gui raylib "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
    elseif(WIN32)
        target_link_libraries(minesweeper-gui raylib winmm gdi32 opengl32)
    else()
        target_link_libraries(minesweeper-gui raylib GL m pthread dl rt X11)
    endif()
endif()

# CLI Targets
if(BUILD_CLI)
    add_executable(minesweeper ${SOURCES-CLI})
    add_executable(minesweeper-benchmark ${SOURCES-BENCHMARK})
    add_executable(minesweeper-animation-demo ${SOURCES-DEMO1})
    add_executable(minesweeper-test ${SOURCES-DEMO2})
endif()

# Enable parallel builds
set(CMAKE_BUILD_PARALLEL_LEVEL 8)