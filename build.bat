@echo off
mkdir build
cd build
cmake .. -DBUILD_CLI=OFF
cmake --build . --config Release
pause
