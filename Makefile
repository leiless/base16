#
# Makefile  Created 190405
#

CC?=gcc
CFLAGS+=-std=c99 -xc -Wall -Wno-unused-parameter -Wextra -Werror

all: debug

base16.o: src/base16.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -c

test.o: test.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -c

test: base16.o test.o
	$(CC) -o $@ $^

release: CFLAGS += -O2
release: test

test-debug: base16.o test.o
	$(CC) -o $@ $^

debug: CPPFLAGS += -DDEBUG
debug: CFLAGS += -g -O0
debug: test-debug

clean:
	rm -f *.o test test-debug

