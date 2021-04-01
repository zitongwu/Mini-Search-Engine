# Makefile for 'crawler' module 
#
# Zitong Wu, Feb 8, 2021

L = ../lib

PROG = crawler
OBJS = crawler.o ../common/pagedir.o
LLIBS = $L/lib.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I../common
MAKE = make
LIBS = -lm

$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

crawler.o: ../common/pagedir.h $(LLIBS) 
..common/pagedir.o: ../common/pagedir.h $(LLIBS) 
.PHONY: test clean

# expects a file `testing.sh` to exist
test: $(PROG) testing.sh
	bash -v testing.sh

valgrind: $(PROG) valgrind_test.sh
	bash valgrind_test.sh

clean:
	rm -f *~ *.o
	rm -f .DS_Store
	rm -f $(PROG)
	rm -f unittest
