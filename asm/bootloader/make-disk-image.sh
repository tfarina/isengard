#!/bin/sh
#
# Create a bootable raw disk image (superfloppy-style) suitable for
# writing to USB or HDD media and booting via legacy BIOS.
#
# The disk contains no partition table. The boot sector is written
# directly to LBA 0.
#

# Die if any command dies.
set -e

IMAGE=disk.img
BOOT=boot.img

SECTOR_SIZE=512
DISK_SIZE_MB=32
DISK_SECTORS=$((DISK_SIZE_MB * 1024 * 1024 / SECTOR_SIZE))

# Sanity checks
[ -f "$BOOT" ] || { echo "Missing $BOOT"; exit 1; }

BOOT_SIZE=$(stat -c %s "$BOOT")
[ "$BOOT_SIZE" -eq 512 ] || {
    echo "boot.img must be exactly 512 bytes (got $BOOT_SIZE)"
    exit 1
}

# Create empty disk image.
#
# The disk size is calculated as follows:
# disk size (MB) × 1024 × 1024 ÷ sector size
# 32 × 1024 × 1024 ÷ 512 = 65536 sectors
dd if=/dev/zero of="$IMAGE" bs=$SECTOR_SIZE count=$DISK_SECTORS

# Write boot sector to LBA 0
dd if="$BOOT" of="$IMAGE" bs=$SECTOR_SIZE conv=notrunc
