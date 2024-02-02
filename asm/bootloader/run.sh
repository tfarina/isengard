#!/bin/sh

# Die if any command dies.
set -e

qemu-system-i386 -drive file=boot.bin,format=raw,index=0,media=disk

