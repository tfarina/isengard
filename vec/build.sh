#!/bin/sh

clang -c os_unix.c -o os_unix.o
clang -c vec.c -o vec.o
clang -c vec_test.c -o vec_test.o

clang os_unix.o vec.o vec_test.o -o vec_test
