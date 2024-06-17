AR := ar
ARFLAGS := cr
CC = gcc
CFLAGS = -g
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
	$(CC) buffer_test.o -L. -lbuffer -o buffer_test

clean:
	$(RM) *.o libbuffer.a buffer_test
