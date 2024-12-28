#! /bin/bash

cmake -B build -D CMAKE_BUILD_TYPE=Debug
cmake --build build --target all