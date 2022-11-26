#!/bin/sh

# Die if any command dies.
set -e

gcc -S -O0 -fno-unwind-tables -fno-asynchronous-unwind-tables -masm=intel add.c -o add_intel_x86.s
