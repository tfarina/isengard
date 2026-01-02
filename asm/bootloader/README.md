Theory
------

When the computer boots, the BIOS does not know how to load an operating
system (OS), so it delegates that task to a boot loader. The boot loader
must therefore be placed in a known, standard location that the BIOS
knows how to access.

On BIOS-based systems, this location is the **first sector of the disk**
(LBA 0), which is exactly 512 bytes in size.

To determine whether a disk is bootable, the BIOS checks that the last
two bytes of this sector contain the boot signature `0x55 0xAA`
(little-endian value `0xAA55`). If the signature is present, the BIOS
loads the 512-byte sector into memory at address `0x7C00` and transfers
execution to it. The code located there is commonly referred to as the
*boot loader*.

This project builds a minimal BIOS boot sector (`boot.img`) and embeds
it into a raw disk image suitable for booting on real hardware or in an
emulator.

### Trying on real hardware

Insert a USB drive and determine its device name (`/dev/sdX`) with:

```sh
sudo lsblk
sudo fdisk -l
```

**Warning:** Writing to /dev/sdX will overwrite the entire device.
Make sure you have selected the correct one.

Create a bootable disk image:
```sh
./make-disk-image.sh
```

Then write the resulting image to the USB drive:
```sh
sudo dd if=disk.img of=/dev/sdX bs=1M status=progress
sync
```

Then:

1. Insert the USB drive into the target computer
2. Power on the machine and enter the boot menu (ofter F12, Esc, or Shift)
3. Select the USB device as the boot source

When finished testing, power off the machine normally.
