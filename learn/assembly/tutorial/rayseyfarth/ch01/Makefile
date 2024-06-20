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
PROGRAMS := exit_example

all: $(LIBRARIES) $(PROGRAMS)

exit_example: exit.o
	$(LD) $(LDFLAGS) -o $@ exit.o $(LIBS)

exit.o: exit.s
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	$(RM) *.o exit_example

.PHONY: all clean
