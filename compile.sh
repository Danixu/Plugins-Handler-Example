#!/bin/bash

# Linux version

# Compile the library
g++ -fPIC -c plugin1.cpp -o plugin1.o
g++ -shared -o plugins/plugin1.so plugin1.o

g++ main.cpp -ldl -std=c++17 -o main


# Windows version

# Compile the library
x86_64-w64-mingw32-g++ -DBUILD_LIB -g -fPIC -c plugin1.cpp -o plugin1.o
x86_64-w64-mingw32-g++ -DBUILD_LIB -g -shared -static-libgcc -static-libstdc++ -o plugins/plugin1.dll plugin1.o

x86_64-w64-mingw32-g++ main.cpp -g -static-libgcc -static-libstdc++ -std=c++17 -o main.exe