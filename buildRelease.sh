#! /bin/bash

cmake -B build -D CMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build build --target all -j1
