#!/bin/bash
gcc -Wall -I./src -c ./src/history.c -o history.o
gcc -Wall -I./src -c ./src/main.c -o main.o
gcc -Wall history.o main.o -o my_shell -lreadline

if [ $? -eq 0 ]; then
    echo "Compilation successful."
    rm history.o main.o
else
    echo "Compilation failed."
fi
