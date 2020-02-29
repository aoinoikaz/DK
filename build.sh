#!/bin/bash

# List of source files and libraries to be compiled with the Game DLL
game_files=./src/game/*.cpp
game_libs=-lSDL2

# Compile dll files to the game DLL
g++ -shared -o lib/game.so -fPIC $game_files $game_libs

# List of source files and libraries to be compiled with the platform layer/main executable
platform_files=./src/platform/*.cpp
platform_libs=-ldl

# Compile main executable and link necessary libraries
g++ -o build/main $platform_files $platform_libs

# Add execution permissions to main executable
chmod a+x build/main