#!/bin/bash

# Optional: clear the terminal
clear

# Compile the C++ program
echo "Compiling the game..."
g++ Terminal_RPG.cpp -o TextAdventure -std=c++17 -pthread

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful! Starting the game..."
    echo ""
    ./TextAdventure
else
    echo "Compilation failed."
fi
