@echo off
set BUILD_TYPE=%~1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Debug

if not exist build mkdir build
cmake -S . -B build -DBUILD_CLI=OFF -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
cmake --build build --config %BUILD_TYPE%
pause
