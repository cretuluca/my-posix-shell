#!/bin/bash
gcc -Wall -I./src -c ./src/main.c -o main.o
gcc -Wall -I./src -c ./src/shell.c -o shell.o
gcc -Wall -I./src -c ./src/builtins.c -o builtins.o
gcc -Wall -I./src -c ./src/utils.c -o utils.o
gcc -Wall -I./src -c ./src/history.c -o history.o
gcc -Wall main.o shell.o builtins.o utils.o history.o -o my_shell -lreadline

if [ $? -eq 0 ]; then
    echo "Compilation successful."
    rm *.o
else
    echo "Compilation failed."
fi