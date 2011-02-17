CC=g++

.SUFFIXES: .cc .o

.cc.o:	
	$(CC) -g -c $*.cc

all: jc jvm ejvm

jc: jc.o stack.o
	$(CC) -o jc -g jc.o stack.o

jvm: jvm.o stack.o
	$(CC) -o jvm -g jvm.o stack.o

ejvm: ejvm.o stack.o
	$(CC) -o ejvm -g ejvm.o stack.o

stack.o: stack.h

jc.o: stack.h
jvm.o: stack.h
ejvm.o: stack.h


clean: cleanjc cleanjvm cleanejvm
	rm -f stack.o

cleanjc:
	rm -f jc jc.o

cleanjvm:
	rm -f jvm jvm.o

cleanejvm:
	rm -f ejvm ejvm.o
