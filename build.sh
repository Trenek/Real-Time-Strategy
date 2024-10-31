#! /bin/bash

cmake -B build
cmake --build build --target all

mkdir build/shaders

glslc ./shaders/shader.vert -o ./build/shaders/vert.spv
glslc ./shaders/shader.frag -o ./build/shaders/frag.spv
