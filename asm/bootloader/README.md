Theory
------

When the computer boots, the BIOS doesn't know how to load the operating system
(OS), so it delegates that task to the boot loader. Thus, the boot loader must
be placed in a known, standard location. That location is the first sector of
the disk (cylinder 0, head 0, sector 0) and it takes 512 bytes.

To make sure that the "disk is bootable", the BIOS checks that bytes
511 and 512 of this boot sector are bytes `0xAA55`. If that's what these last
two bytes are, the BIOS moves the 512 bytes to the memory address 0x7C00 and
treats whatever was at the beginning of the 512 bytes as code, the so-called "bootloader".

### Trying on a real hardware

Insert an USB drive and determine its device (`/dev/sdX`) with:

```
sudo lsblk
sudo fdisk -l
```

Pick the binary you want to run and:

```
sudo dd if=boot.img of=/dev/sdX
```

Then:

1. Insert the USB in the computer
2. During boot, hit some special key, usually F12, Esc, Shift
3. Choose to boot from the USB

When you are done, just hit the power button to shutdown and turn the computer
again.
