#!/bin/sh

mkdir -p out/

clang -c sbuf.c -o out/sbuf.o

rm -rf out/libsbuf.a && ar rcs out/libsbuf.a out/sbuf.o

clang -o out/sbuf_test sbuf_test.c -Lout/ -lsbuf
