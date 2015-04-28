#!/bin/sh

mkdir -p out/

clang -c unix.c -o out/unix.o
clang -c buf.c -o out/buf.o

rm -rf out/libbuf.a && ar rcs out/libbuf.a out/unix.o out/buf.o

clang -o out/buf_test buf_test.c -Lout/ -lbuf
