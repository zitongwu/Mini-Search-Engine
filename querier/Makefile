# Makefile for 'querier' module 
# compiles querier and fuzzquery
# includes unit test (using both fuzzquey and self-created query examples) 
# and valgrind test (with both fuzzquey and self-created query examples) 
# Zitong Wu, Feb 22, 2021


L1 = ../lib
L2 = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$(L1) -I$(L2)
MAKE = make
LIBS = -lm
LLIBS = $(L1)/lib.a
LLIBS2 = $(L2)/common.a

.PHONY: all test valgrind clean 

all: querier fuzzquery

querier: querier.o counters_helper.o
	$(CC) $(CFLAGS) $^ $(LLIBS2) $(LLIBS) $(LIBS) -o $@
fuzzquery: fuzzquery.o
	$(CC) $(CFLAGS) $^ $(LLIBS2) $(LLIBS) $(LIBS) -o $@

counters_helper.o: counters_helper.h

valgrind: querier fuzzquery
	valgrind ./fuzzquery indexFile 10 0 | ./querier data/depth2 indexFile > /dev/null
	valgrind ./querier data/depth2 indexFile < incorrectQueries > /dev/null
	valgrind ./querier data/depth2 indexFile < correctQueries > /dev/null


test: querier fuzzquery testing.sh
	bash -v testing.sh

clean:
	rm -f *~ *.o
	rm -f .DS_Store
	rm -f querier
	rm -f indexFile2
	rm -f fuzzquery