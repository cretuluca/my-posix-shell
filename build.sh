#!/bin/bash

gcc -Wall ./src/main.c -o my_shell

if [ $? -eq 0 ]; then
    echo "Compilation successful."
else
    echo "Compilation failed."
fi