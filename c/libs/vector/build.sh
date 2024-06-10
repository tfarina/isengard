#!/bin/sh

mkdir -p out/

gcc -c vector.c -o out/vector.o

rm -rf out/libvector.a && ar rcs out/libvector.a out/vector.o

gcc -o out/vector_test vector_test.c -Lout/ -lvector
