AS := as
ASFLAGS := --32
LOAD_ADDR := 0x7c00
LD := ld
LDFLAGS := --oformat binary -e _start -Ttext $(LOAD_ADDR)
RM := rm -f

boot.bin: boot.o
	$(LD) $(LDFLAGS) -o $@ $<
	@SIZE=$$(stat --format='%s' $@); \
	if [ $$SIZE -ne 512 ]; then \
		echo "Error: boot.bin is $$SIZE bytes (must be 512)"; exit 1; \
	fi

boot.o: boot.S
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	$(RM) *.o boot.bin

.PHONY: clean
