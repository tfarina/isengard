#!/bin/sh

# Compile source files into object files.
clang -g -std=c99 -pedantic -Wall -O0 -c md5.c -o md5.o
clang -g -std=c99 -pedantic -Wall -O0 -c main.c -o main.o

# Link the object files into an executable.
clang -o md5 md5.o main.o
