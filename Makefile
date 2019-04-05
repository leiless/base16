#
# Makefile  Created 190405
#

CC?=gcc
CFLAGS+=-std=c99 -xc -Wall -Wno-unused-parameter -Wextra -Werror

all: debug

test.o: test.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -c

test: test.o
	$(CC) -o $@ $^

release: CFLAGS += -O2
release: test

test-debug: test.o
	$(CC) -o $@ $^

debug: CPPFLAGS += -DDEBUG
debug: CFLAGS += -g -O0
debug: test-debug

clean:
	rm -f *.o test test-debug a.out

