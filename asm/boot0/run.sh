#!/bin/sh

# Die if any command dies.
set -e

qemu-system-i386 boot0.bin

