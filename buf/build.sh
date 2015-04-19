#!/bin/sh

clang -c unix.c -o unix.o
clang -c buf.c -o buf.o
rm -rf libbuf.a && ar rcs libbuf.a unix.o buf.o

clang -o buf_test buf_test.c -L. -lbuf
