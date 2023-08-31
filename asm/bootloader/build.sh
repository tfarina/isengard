#!/bin/sh

# Die if any command dies.
set -e

as -o boot0.o boot0.s
ld -o boot0.bin --oformat binary -e _start -Ttext 0x7c00 boot0.o
