#!/bin/sh

mkdir -p out/

clang -c buffer.c -o out/buffer.o

rm -rf out/libbuffer.a && ar rcs out/libbuffer.a out/buffer.o

clang -o out/buffer_test buffer_test.c -Lout/ -lbuffer
