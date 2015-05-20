#!/bin/sh

# Die if any command dies.
set -e

as -o boot0.o boot0.s
ld -Ttext 0x7c00 -oformat=binary -o boot0.bin boot0.o
