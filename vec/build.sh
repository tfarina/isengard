#!/bin/sh

clang -c os_unix.c -o os_unix.o
clang -c vec.c -o vec.o
clang -c main.c -o main.o

clang os_unix.o vec.o main.o -o main
