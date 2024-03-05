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
	mov $hello_string, %si	# set SI to the address of the first byte of our string

print_char:
	lodsb			# load the memory byte from the address in SI into the AL register and increments SI
	cmp $0, %al		# compare the constant 0, with the content of the AL register
	je done			# if al == 0, jump to "done"
	int $0x10		# call function in AH register to write character in the screen
	jmp print_char		# jump to "print_char", effectively looping to the next char

done:
	hlt			# stop executing

hello_string:	.asciz "Hello, world!"

	.fill 510-(.-_start), 1, 0	# add zeroes to make it 510 bytes long
	.word 0xAA55		# append bootloader signature
