RM := rm -f

boot.o: boot.S
	as -o $@ $<

boot.bin: boot.o
	ld -o $@ --oformat binary -e _start -Ttext 0x7c00 $<

clean:
	$(RM) *.o boot.bin

.PHONY: clean
