AR := ar
ARFLAGS := cru
CC = gcc
CFLAGS = -g
LDFLAGS := -L.
LIBS := -lbuffer
RANLIB := ranlib
RM := rm -f
LIBRARIES := libbuffer.a
PROGRAMS := buffer_test

all: $(LIBRARIES) $(PROGRAMS)

libbuffer.a: buffer.o
	$(RM) libbuffer.a
	$(AR) $(ARFLAGS) libbuffer.a buffer.o
	$(RANLIB) libbuffer.a

buffer_test: buffer_test.o libbuffer.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ buffer_test.o $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) *.o $(LIBRARIES) $(PROGRAMS)

.PHONY: all clean
