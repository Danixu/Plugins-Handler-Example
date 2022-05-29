#!/bin/bash

# Linux version
# Compile the library. Linux version doesn't requires the export.cpp file because there are no replaced functions.
g++ -fPIC -shared -o plugins/plugin1.so plugin1.cpp
# Main program
g++ main.cpp plugin_handler.cpp -ldl -std=c++17 -o main


# Windows version
# Compile the library. dlerror function is replaced in windows, so export.cpp is required
x86_64-w64-mingw32-g++ -fPIC -shared -DBUILD_LIB -static-libgcc -static-libstdc++ -o plugins/plugin1.dll export.cpp plugin1.cpp
# Main Program
x86_64-w64-mingw32-g++ export.cpp main.cpp plugin_handler.cpp -static-libgcc -static-libstdc++ -std=c++17 -o main.exe