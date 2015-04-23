#!/bin/sh

clang -c unix.c -o unix.o
clang -c buf.c -o buf.o
mkdir -p out/
rm -rf out/libbuf.a && ar rcs out/libbuf.a unix.o buf.o

clang -o buf_test buf_test.c -Lout/ -lbuf
