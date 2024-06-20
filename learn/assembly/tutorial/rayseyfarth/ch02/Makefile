AR := ar
ARFLAGS := cru
AS = as
ASFLAGS =
CC = gcc
CFLAGS = -g
LD := ld
LDFLAGS :=
LIBS :=
RANLIB := ranlib
RM := rm -f
LIBRARIES :=
PROGRAMS := fp_example

all: $(LIBRARIES) $(PROGRAMS)

fp_example: fp.o
	$(LD) $(LDFLAGS) -o $@ fp.o $(LIBS)

fp.o: fp.s
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	$(RM) *.o fp_example

.PHONY: all clean
