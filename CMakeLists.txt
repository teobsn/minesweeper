cmake_minimum_required(VERSION 3.15)

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
    # Force Release CRT for non-debug builds to avoid ucrtbased.dll dependency
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    add_compile_options(/W4 /O2 /permissive- /DNDEBUG)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64")
        add_compile_options(/arch:AVX2)
    endif()
else()
    add_compile_options(-Wall -Wextra -O3 -flto)
    if(NOT APPLE)
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "amd64")
            add_compile_options(-mavx2 -march=native)
        endif()
    endif()
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
if(RAYLIB_INCLUDE_PATH)
    include_directories(${RAYLIB_INCLUDE_PATH})
endif()
if(RAYLIB_LIB_PATH)
    link_directories(${RAYLIB_LIB_PATH})
endif()

# Source groups
set(SOURCES-COMMON
    src/cell.cpp
    src/game.cpp
    src/map.cpp
    src/logger.cpp
    src/stats.cpp
    src/powerups.cpp
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
        ${SOURCES-COMMON}
        src/benchmark.cpp
        src/cli.cpp
    )

    set(SOURCES-DEMO1
        ${SOURCES-COMMON}
        src/animation-demo.cpp
        src/cli.cpp
    )

    set(SOURCES-DEMO2
        ${SOURCES-COMMON}
        src/test-demo.cpp
        src/cli.cpp
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
