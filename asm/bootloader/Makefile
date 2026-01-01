AS := as
ASFLAGS := --32
LOAD_ADDR := 0x7c00
LD := ld
LDFLAGS := --oformat binary -e _start -Ttext $(LOAD_ADDR)
RM := rm -f

boot.bin: boot.o
	$(LD) $(LDFLAGS) -o $@ $<

boot.o: boot.S
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	$(RM) *.o boot.bin

.PHONY: clean
