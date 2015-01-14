#!/bin/sh

clang -c unix.c -o unix.o
clang -c buf.c -o buf.o
clang -c buf_test.c -o buf_test.o

clang unix.o buf.o buf_test.o -o buf_test
