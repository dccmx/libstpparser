CC?=gcc
AR?=ar

CPPFLAGS += -I. -I./tests

CFLAGS += -Wall -Wextra -Werror -fPIC -g

all: lib run_test

lib: libstpparser.a

libstpparser.a: stpparser.o
	$(AR) rcs libstpparser.a stpparser.o

stpparser.o: stpparser.c stpparser.h Makefile
	$(CC) $(CPPFLAGS) $(CFLAGS) -g -c stpparser.c

run_test: stpparser.c stpparser.h tests/test.c Makefile tests/test_*.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -g -o run_test stpparser.c tests/test.c

test: run_test
	./run_test

distclean: clean

clean:
	rm -rf *.o *.a test *.dSYM
