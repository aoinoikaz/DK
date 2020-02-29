#!/bin/bash

# List of source files to be compiled with the Game DLL
dll_files=src/game.cpp

# Compile dll files to game.so
g++ -shared -o lib/game.so -fPIC $dll_files

# List of source files to be compiled with the platform layer/main executable
cpp_files=src/main.cpp

# Compile main executable and link necessary libraries
g++ -o build/main $cpp_files -ldl

# Add execution permissions to main executable
chmod a+x build/main