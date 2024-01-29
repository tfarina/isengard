#
# Code from:
# www.codeproject.com/Articles/664165/Writing-a-boot-loader-in-Assembly-and-C-Part
# https://medium.com/@g33konaut/writing-an-x86-hello-world-boot-loader-with-assembly-3e4c5bdd96cf
#
# If the bootloader signature bytes are 0x55aa, why are we swapping them down
# below? That is because x86 (the machine I'm coding this on) is little endian,
# so bytes get swapped in memory, and thus we have to invert and write them as
# 0xaa55. Otherwise, qemu gives the error "Boot failed: not a bootable disk".
# https://ftp.gnu.org/old-gnu/Manuals/gas-2.9.1/html_chapter/as_7.html
# https://github.com/rofl0r/endianness.h/blob/master/endianness.h
#

.code16 			# generate 16-bit code

.text   			# executable code location
        .globl _start;

_start: 			# code entry point
	mov $0x0e, %ah		# set AH to 0x0e (function Teletype)
	mov $hello_string, %bx	# set BX to the address of the first byte of our string
	mov (%bx), %al		# set AL to the first byte of our message
	int $0x10		# call the function in AH from interrupt 0x10
	hlt			# stop executing

hello_string:	.asciz "Hello, world!"

	.fill 510-(.-_start), 1, 0	# add zeroes to make it 510 bytes long
	.word 0xaa55		# append bootloader signature
