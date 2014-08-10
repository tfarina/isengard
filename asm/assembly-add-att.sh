#!/bin/sh

# Die if any command dies.
set -e

clang -S -O0 -fno-unwind-tables -fno-asynchronous-unwind-tables add.c -o add-att.s

echo 'now edit add-att.s to remove the clutter'
