#!/bin/sh

# Die if any command dies.
set -e

as -o boot.o boot.s
ld -o boot.bin --oformat binary -e _start -Ttext 0x7c00 boot.o
