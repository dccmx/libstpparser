CC?=gcc
AR?=ar

CPPFLAGS += -I. -I./tests

CFLAGS += -Wall -Wextra -Werror -fPIC -g

all: lib test

lib: libstpparser.a

libstpparser.a: stpparser.o
	$(AR) rcs libstpparser.a stpparser.o

stpparser.o: stpparser.c stpparser.h Makefile
	$(CC) $(CPPFLAGS) $(CFLAGS) -c stpparser.c

test: stpparser.c stpparser.h tests/test.c Makefile tests/test_*.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o test stpparser.c tests/test.c

distclean: clean

clean:
	rm -rf *.o *.a test *.dSYM
