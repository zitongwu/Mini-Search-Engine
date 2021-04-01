# Makefile for the 'common' module 
# Builds and compiles all the utility functions into a library common.a:
# 	pagedir.o word.o index.o filepath.o
# Zitong Wu, Feb 8, 2021


OBJS = pagedir.o word.o index.o filepath.o
L = ../lib
LIB = common.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L 
MAKE = make
LIBS = -lm
LLIBS = $L/lib.a

.PHONY: clean sourcelist

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h 
word.o: word.h
index.o: index.h
filepath.o: filepath.h

sourcelist: Makefile *.md *.c *.h
	@echo $^

clean:
	rm -f .DS_Store
	rm -f core
	rm -f *~ *.o
	rm -f $(LIB) *~ *.o
