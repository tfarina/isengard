hello_x86: hello_x86.o
	ld -m elf_i386 -o $@ $<

hello_x86.o: hello_x86.s
	as --32 -o $@ $<
