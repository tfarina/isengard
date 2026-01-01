#!/bin/sh

# Die if any command dies.
set -e

IMAGE=floppy.img
BOOT=boot.img
SECTOR_SIZE=512
FLOPPY_SECTORS=2880

# Sanity checks
[ -f "$BOOT" ] || { echo "Missing $BOOT"; exit 1; }

BOOT_SIZE=$(stat -c %s "$BOOT")
[ "$BOOT_SIZE" -le 512 ] || {
    echo "boot.img must be ≤ 512 bytes (got $BOOT_SIZE)"
    exit 1
}

# Create empty 1.44MB floppy image.
#
# The classic 3.5" floppy size (1.44MB) is calculated as follows:
# floppy sectors x sector size
# 2880 × 512 = 1,474,560 bytes
dd if=/dev/zero of="$IMAGE" bs=$SECTOR_SIZE count=$FLOPPY_SECTORS

# Write boot image to the first sector
dd if="$BOOT" of="$IMAGE" bs=$SECTOR_SIZE conv=notrunc
