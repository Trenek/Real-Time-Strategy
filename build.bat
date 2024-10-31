@echo off

cmake -B build -G "MinGW Makefiles"
cmake --build build --target all -j1

mkdir build\shaders

glslc shaders\shader.vert -o build\shaders\vert.spv
glslc shaders\shader.frag -o build\shaders\frag.spv

pause
