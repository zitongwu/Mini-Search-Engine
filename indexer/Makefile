# Makefile for 'indexer' module 
# compiles indexer and indextest
# includes unit test and valgrind test for both indexer and indextest
# Zitong Wu, Feb 15, 2021


L1 = ../lib
L2 = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$(L1) -I$(L2)
MAKE = make
LIBS = -lm
LLIBS = $(L1)/lib.a
LLIBS2 = $(L2)/common.a

.PHONY: all test clean 

all: indexer indextest

indexer: indexer.o 
	$(CC) $(CFLAGS) $^ $(LLIBS2) $(LLIBS) $(LIBS) -o $@
indextest: indextest.o 
	$(CC) $(CFLAGS) $^ $(LLIBS2) $(LLIBS) $(LIBS) -o $@


# expects a file `testing.sh` to exist
test: indexer testing.sh indextest
	bash -v testing.sh

valgrind: indexer indextest ../crawler/data/depth1
	valgrind ./indexer ../crawler/data/depth1 valgrindIndexfile
	valgrind ./indextest valgrindIndexfile cp_valgrindIndexfile

clean:
	rm -f *~ *.o
	rm -f .DS_Store
	rm -f indextest
	rm -f indexer
	rm -f newIndexFile.sorted
	rm -f oldIndexFile.sorted
	rm -f newIndexFile
	rm -f oldIndexFile
	rm -f valgrindIndexfile
	rm -f cp_valgrindIndexfile
	rm -f depth0IndexFile
	rm -f depth0IndexFile.sorted
	rm -f depth1IndexFile
	rm -f depth1IndexFile.sorted
