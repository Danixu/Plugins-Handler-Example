#!/bin/bash

mkdir -p build/bin/plugins

# Linux version
echo "Compiling linux version..."
# Compile the library. Linux version doesn't requires the export.cpp file because there are no replaced functions.
echo "Shared library (.so)"
g++ -fPIC -shared -I../common/include -Iinclude src/plugin1.cpp -o build/bin/plugins/plugin1.so
# Main program
echo "Main program"
g++ -I../common/include -Iinclude src/plugin_handler.cpp src/test_executable.cpp -ldl -std=c++17 -o build/bin/test_executable


# Windows version
echo "Compiling windows version..."
# Compile the library. dlerror function is replaced in windows, so export.cpp is required
echo "Shared library (.dll)"
x86_64-w64-mingw32-g++ -fPIC -shared -DBUILD_LIB -I../common/include -Iinclude ../common/src/export.cpp src/plugin1.cpp -static-libgcc -static-libstdc++ -o build/bin/plugins/plugin1.dll
# Main Program
echo "Main program"
x86_64-w64-mingw32-g++ -I../common/include -Iinclude ../common/src/export.cpp src/plugin_handler.cpp src/test_executable.cpp -static-libgcc -static-libstdc++ -std=c++17 -o build/bin/test_executable.exe