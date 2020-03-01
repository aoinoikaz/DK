#!/bin/bash

# List of source files and libraries to be compiled with the Game DLL
game_files=./src/game/*.cpp
# game_libs=""

# Compile dll files to the game DLL
g++ -shared -o lib/game.so -fPIC $game_files $game_libs

# List of source files and libraries to be compiled with the platform layer/main executable
platform_files=./src/platform/*.cpp
platform_libs="-ldl -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gpu -lGL"

# SDL2 config flags
sdl2_config_flag=`sdl2-config --cflags --libs`

# Compile main executable and link necessary libraries
g++ -o build/main $platform_files $sdl2_config_flag $platform_libs

# Add execution permissions to main executable
chmod a+x build/main