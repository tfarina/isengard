CC = gcc
CFLAGS = -g

RM := rm -f

buffer.o: buffer.c
	$(CC) $(CFLAGS) -c -o $@ $<

buffer_test.o: buffer_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

libbuffer: buffer.o
	$(RM) libbuffer.a && ar rcs libbuffer.a buffer.o

buffer_test: libbuffer buffer_test.o
	$(CC) buffer_test.o -L. -lbuffer -o buffer_test

clean:
	$(RM) *.o libbuffer.a buffer_test
