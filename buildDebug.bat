@echo off

cmake -B build -G "MinGW Makefiles" -D CMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build build --target all -j1

pause
