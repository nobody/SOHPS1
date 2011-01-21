# Example makefile
# Edwin Sha: CSE341
#
# This is the makefile for assignment 1 -- it provides a way to 
# make the test programs "test" (which was provided by nachos),
# "jc".  Use "make" or "gmake" with this makefile
#
# If any of this is foreign to you, read the man page fore make.


# This lets you define a generic compile for .cc files

CC=g++

.SUFFIXES: .cc .o

.cc.o:	
	$(CC) -g -c $*.cc

all: simpletest jc jvm ejvm

simpletest: stack.o simpletest.o
	$(CC) -o simpletest -g simpletest.o stack.o

jc: jc.o stack.o
	$(CC) -o jc -g jc.o stack.o

jvm: jvm.o stack.o
	$(CC) -o jvm -g jvm.o stack.o

ejvm: ejvm.o stack.o
	$(CC) -o ejvm -g ejvm.o stack.o

test.o: stack.h
stack.o: stack.h

jc.o: stack.h
jvm.o: stack.h
ejvm.o: stack.h


