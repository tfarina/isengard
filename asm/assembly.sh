#!/bin/sh

# Die if any command dies.
set -e

clang -S -O0 -fno-unwind-tables -fno-asynchronous-unwind-tables add.c -o add_att_x64.s
clang -S -O0 -fno-unwind-tables -fno-asynchronous-unwind-tables -masm=intel add.c -o add_intel_x64.s

echo 'now edit add_att_x64.s to remove the cfi directives'
echo 'now edit add_intel_x64.s to remove the cfi directives'
