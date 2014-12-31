#!/bin/sh

# Die if any command dies.
set -e

clang -S -O0 -fno-unwind-tables -fno-asynchronous-unwind-tables add.c -o add_att_x64.s

echo 'now edit add_att_x64.s to remove the clutter'
