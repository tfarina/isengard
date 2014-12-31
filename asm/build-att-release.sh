#!/bin/sh

# Die if any command dies.
set -e

# yasm -f elf64 -p gas add_att_x64.s -o add_def.o

as add_att_x64.s -o add_def.o
clang++ -c add_main.cc -o add_main.o
clang++ add_main.o add_def.o -o add_att_release
