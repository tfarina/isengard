#!/bin/sh

# Die if any command dies.
set -e

dd if=/dev/zero of=floppy.img bs=512 count=2880
dd if=boot0.bin of=floppy.img
