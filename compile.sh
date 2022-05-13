#!/bin/bash

# Compile the library
g++ -fPIC -c plugin1.cpp -o plugin1.o
g++ -shared -o plugins/plugin1.so plugin1.o

g++ main.cpp -ldl -o main
