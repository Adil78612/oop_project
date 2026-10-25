#!/usr/bin/env bash
# Build the game (MSYS2 UCRT64), then run it.
set -e

g++ -std=c++17 main.cpp Player.cpp Enemy.cpp Brute.cpp Bullet.cpp Weapon.cpp Pickup.cpp \
    -o main.exe -lraylib -lopengl32 -lgdi32 -lwinmm

./main.exe
