// To compile the stack test, do "g++ stack.cc simpletest.cc".

// This is needed to tell the compiler that the stdio functions and the
// exit function are compiled by a C compiler.  On many systems, the header
// files are C++-ified already so this isn't necessary.

extern "C" {
#include <stdio.h>
extern void exit(int st);
}

#include "stack.h"

// The test routine will push 10 numbers, starting with 17, onto the stack,
// and then print them as it pops them off.

int
main(int argc, char* argv[])
{
    Stack stack(50000);   // Constructor with an argument.
    int count = 17;
    
    // Put a bunch of stuff in the stack...
    while (!stack.Full())
	stack.Push(count++);
    
    // ... and take it out again.
    while (!stack.Empty())
	printf("pop %d\n", stack.Pop());
    
    exit(0);
}
