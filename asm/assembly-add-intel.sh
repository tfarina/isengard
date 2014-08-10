#!/bin/sh

# Die if any command dies.
set -e

clang -S -O0 -fno-unwind-tables -fno-asynchronous-unwind-tables -masm=intel add.c -o add-intel.s

echo 'now edit add-intel.s to remove the clutter'
