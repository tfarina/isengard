#!/bin/sh

# Die if any command dies.
set -e

# yasm -f elf64 -p gas -o add_def.o add_att_x64.s

as -o add_def.o add_att_x64.s
clang -g -c add_main.c -o add_main.o
clang -o add_att_debug add_def.o add_main.o
