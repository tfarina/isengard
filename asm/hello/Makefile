RM := rm -f

hello_x86: hello_x86.o
	gcc -m32 -nostdlib -o $@ $<

hello_x86.o: hello_x86.s
	gcc -m32 -c -o $@ $<

hello_x64: hello_x64.o
	ld -m elf_x86_64 -o $@ $<

hello_x64.o: hello_x64.s
	as --64 -o $@ $<

clean:
	$(RM) *.o hello_x86
