#!/bin/sh

clang -c md5.c -o md5.o
clang -c main.c -o main.o
clang -o md5 md5.o main.o
