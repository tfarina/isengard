CC = gcc
CFLAGS = -g

buffer.o: buffer.c
	$(CC) $(CFLAGS) -c $< -o $@

libbuffer: buffer.o
	rm -rf libbuffer.a && ar rcs libbuffer.a buffer.o

buffer_test: libbuffer buffer_test.c
	$(CC) buffer_test.c -L. -lbuffer -o buffer_test

clean:
	rm -f buffer.o libbuffer.a buffer_test
