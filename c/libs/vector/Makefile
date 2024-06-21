AR := ar
ARFLAGS := cru
CC = gcc
CFLAGS = -g
LDFLAGS := -L.
LIBS := -lvector
RANLIB := ranlib
RM := rm -f
LIBRARIES := libvector.a
PROGRAMS := vector_unittests vector_example

all: $(LIBRARIES) $(PROGRAMS)

libvector.a: vector.o
	$(RM) libvector.a
	$(AR) $(ARFLAGS) libvector.a vector.o
	$(RANLIB) libvector.a

vector_unittests: tests/run_all_unittests.o tests/vector_test.o libvector.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ tests/run_all_unittests.o tests/vector_test.o $(LIBS)

vector_example: examples/vector_example.o libvector.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ examples/vector_example.o $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) *.o tests/*.o examples/*.o $(LIBRARIES) $(PROGRAMS)

.PHONY: all clean
