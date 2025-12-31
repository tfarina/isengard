RM := rm -f

hello_x86: hello_x86.o
	gcc -m32 -nostdlib -o $@ $<

hello_x86.o: hello_x86.s
	gcc -m32 -c -o $@ $<

clean:
	$(RM) *.o hello_x86
