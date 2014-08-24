#!/bin/sh

clang -c unix.c -o unix.o
clang -c buf.c -o buf.o
clang -c main.c -o main.o

clang unix.o buf.o main.o -o main
