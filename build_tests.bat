@echo off

cmake -B tests_build -G "MinGW Makefiles" -D CMAKE_BUILD_TYPE=Debug
cmake --build tests_build --target Tester -j1

echo:
echo Testy zostały zbudowane.
pause