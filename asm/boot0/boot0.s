#
# Code from:
# www.codeproject.com/Articles/664165/Writing-a-boot-loader-in-Assembly-and-C-Part
#

.code16 			# generate 16-bit code

.text   			# executable code location
        .globl _start;

_start: 			# code entry point
	jmp _start
	.fill 510-(.-_start), 1, 0
        .byte 0x55		# append boot signature
        .byte 0xaa 		# append boot signature
