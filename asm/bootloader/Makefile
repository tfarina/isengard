AS := as
ASFLAGS := --32
RM := rm -f

boot.bin: boot.o
	ld -o $@ --oformat binary -e _start -Ttext 0x7c00 $<

boot.o: boot.S
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	$(RM) *.o boot.bin

.PHONY: clean
