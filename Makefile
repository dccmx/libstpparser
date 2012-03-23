CC?=gcc
AR?=ar

CPPFLAGS += -I.

CFLAGS += -Wall -Wextra -Werror -fPIC -g

lib: stpparser.o
	$(AR) rcs libstpparser.a stpparser.o

stp_parser.o: stpparser.c stpparser.h Makefile
	$(CC) $(CPPFLAGS) $(CFLAGS) -c stpparser.c

test: stpparser.c stpparser.h test.c Makefile
	$(CC) $(CPPFLAGS) $(CFLAGS) -o test stpparser.c test.c

clean:
	rm -f *.o *.a test
