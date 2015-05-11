#!/bin/sh

# Die if any command dies.
set -e

# yasm -f elf64 -p gas -o add_def.o add_att_x64.s

as add_att_x64.s -o add_def.o
clang -g -c add_main.c -o add_main.o
clang add_main.o add_def.o -o add_att_debug
