AS := as
ASFLAGS := --32
LD := ld
LDFLAGS := --oformat binary -e _start -Ttext 0x7c00
RM := rm -f

boot.bin: boot.o
	$(LD) -o $@ $(LDFLAGS) $<

boot.o: boot.S
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	$(RM) *.o boot.bin

.PHONY: clean
