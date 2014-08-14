#!/bin/sh

clang -c unix.c -o unix.o
clang -c vec.c -o vec.o
clang -c main.c -o main.o

clang unix.o vec.o main.o -o main
