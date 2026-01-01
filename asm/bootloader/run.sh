#!/bin/sh

# Die if any command dies.
set -e

BOOT=boot.img

if [ ! -f "$BOOT" ]; then
    echo "error: $BOOT not found"
    exit 1
fi

qemu-system-i386 \
    -drive file="$BOOT",format=raw,index=0,media=disk \
    -no-reboot \
    -no-shutdown
