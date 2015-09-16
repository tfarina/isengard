#!/bin/sh

clang -c md5.c -o md5.o
clang -o md5 main.c md5.o
