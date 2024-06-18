AR := ar
ARFLAGS := cru
CC = gcc
CFLAGS = -g
LDFLAGS := -L.
LIBS := -lbuffer
RANLIB := ranlib
RM := rm -f

buffer.o: buffer.c
	$(CC) $(CFLAGS) -c -o $@ $<

buffer_test.o: buffer_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

libbuffer.a: buffer.o
	$(RM) libbuffer.a
	$(AR) $(ARFLAGS) libbuffer.a buffer.o
	$(RANLIB) libbuffer.a

buffer_test: buffer_test.o libbuffer.a
	$(CC) $(LDFLAGS) -o $@ buffer_test.o $(LIBS)

clean:
	$(RM) *.o libbuffer.a buffer_test

.PHONY: clean
