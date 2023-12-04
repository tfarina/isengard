#
# Code from:
# www.codeproject.com/Articles/664165/Writing-a-boot-loader-in-Assembly-and-C-Part
# https://medium.com/@g33konaut/writing-an-x86-hello-world-boot-loader-with-assembly-3e4c5bdd96cf
#

.code16 			# generate 16-bit code

.text   			# executable code location
        .globl _start;

_start: 			# code entry point
	mov $0x0e41, %ax	# set AH to 0x0e (function Teletype) and AL to 0x41 (ASCII for "A")
	int $0x10		# call the function in AH from interrupt 0x10
	hlt			# stop executing

	.fill 510-(.-_start), 1, 0	# add zeroes to make it 510 bytes long
	.word 0xaa55		# append bootloader signature
