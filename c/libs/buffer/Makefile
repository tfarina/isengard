AR := ar
ARFLAGS := cru
CC = gcc
CFLAGS = -g
LDFLAGS := -L.
LIBS := -lbuffer
RANLIB := ranlib
RM := rm -f
LIBRARIES := libbuffer.a
PROGRAMS := buffer_unittests buffer_example

all: $(LIBRARIES) $(PROGRAMS)

libbuffer.a: buffer.o
	$(RM) libbuffer.a
	$(AR) $(ARFLAGS) libbuffer.a buffer.o
	$(RANLIB) libbuffer.a

buffer_unittests: run_all_unittests.o buffer_test.o libbuffer.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ run_all_unittests.o buffer_test.o $(LIBS)

buffer_example: buffer_example.o libbuffer.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ buffer_example.o $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) *.o $(LIBRARIES) $(PROGRAMS)

.PHONY: all clean
