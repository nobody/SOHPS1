#include<stdio.h>
#include<iostream>
#include<sstream>
#include<string.h>
#include<map>

#include "stack.h"

#define MAX_COMM_LEN 100

using namespace std;

void runOpcode(char* opcode, Stack &stack, map<unsigned int, int> &vars){

    stringstream ss;
    char command[MAX_COMM_LEN];
    memset(command, 0, MAX_COMM_LEN);

    ss << opcode;
    ss >> command;

    if (strcmp("const", command) == 0){
        // push constant arg onto stack
        int arg = 0;
        ss >> arg;
        if (ss.fail()){
            ss.clear();
            char arg[25];
            memset(arg, 0, 25);
            ss >> arg;

            printf("Error reading augument: \"%s\".  Not a number.\n", arg);

        } else {
            stack.Push(arg);

#ifdef DEBUG
            printf("Pushed constant \"%d\" onto stack\n", arg);
#endif
        }
    } else if (strcmp("pop", command) == 0){
        // pop the stack and discard
        int popped = stack.Pop();
        
#ifdef DEBUG
        printf("Popped \"%d\" from the stack\n", popped);
#endif

    } else if (strcmp("store", command) == 0){
        // pop stack, store value in variable "arg"

        int arg = 0;
        ss >> arg;
        if (ss.fail()){
            ss.clear();
            char arg[25];
            memset(arg, 0, 25);
            ss >> arg;
            printf("Error reading augument: \"%s\".  Not a number.\n", arg);
        } else {
            if (arg < 0) {
                fprintf(stderr, "error: variable index should be non-negative\n");
            } else {
                if (stack.Empty()){
                    fprintf(stderr, "error: stack empty\n");
                } else {
                    int val = stack.Pop();
                    vars[arg] = val;
#ifdef DEBUG
                    printf("Set var %d to value %d\n", arg, val);
#endif
                }
            }
        }

    } else if (strcmp("load", command) == 0){
        // push value in variable "arg" onto stack

        int arg = 0;
        ss >> arg;
        if (ss.fail()){
            ss.clear();
            char arg[25];
            memset(arg, 0, 25);
            ss >> arg;
            printf("Error reading augument: \"%s\".  Not a number.\n", arg);
        } else {
            if (arg < 0){
                fprintf(stderr, "error: variable index should be non-negative\n");
            } else {
                if (stack.Full()){
                    fprintf(stderr, "error: stack full\n");
                } else {
                    stack.Push(vars[arg]);
#ifdef DEBUG
                    printf("Pushed var %d (value %d) onto stack\n", arg, vars[arg]);
#endif
                }
            }
        }

    } else if (strcmp("add", command) == 0){
        // pop 2 ints, add and push result
        
        if (stack.Empty()){
            fprintf(stderr, "error: stack empty\n");
        } else {
            int arg1 = stack.Pop();
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
                stack.Push(arg1);
            } else {
                int arg2 = stack.Pop();

                stack.Push(arg1+arg2);

#ifdef DEBUG
                printf("Added %d and %d, pushed %d\n", arg1, arg2, arg1+arg2);
#endif
            }
        }

    } else if (strcmp("sub", command) == 0){
        // pop 2 ints, subtract second from first, push result

        if (stack.Empty()){
            fprintf(stderr, "error: stack empty\n");
        } else {
            int arg1 = stack.Pop();
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
                stack.Push(arg1);
            } else {
                int arg2 = stack.Pop();

                stack.Push(arg2-arg1);

#ifdef DEBUG
                printf("Subtracted %d and %d, pushed %d\n", arg2, arg1, arg2-arg1);
#endif
            }
        }
    } else if (strcmp("mul", command) == 0){
        // pop 2 ints, multiply and push the result

        if (stack.Empty()){
            fprintf(stderr, "error: stack empty\n");
        } else {
            int arg1 = stack.Pop();
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
                stack.Push(arg1);
            } else {
                int arg2 = stack.Pop();

                stack.Push(arg1*arg2);

#ifdef DEBUG
                printf("Multiplied %d and %d, pushed %d\n", arg1, arg2, arg1*arg2);
#endif
            }
        }
    } else if (strcmp("div", command) == 0){
        // pop 2 ints, divide 2/1, and push the result -- TODO: Document this choice

        if (stack.Empty()){
            fprintf(stderr, "error: stack empty\n");
        } else {
            int arg1 = stack.Pop();
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
                stack.Push(arg1);
            } else {
                int arg2 = stack.Pop();

                if (arg1 == 0){
                    fprintf(stderr, "error: division by zero\n");
                    stack.Push(arg2);
                    stack.Push(arg1);
                } else {
                    stack.Push(arg2/arg1);

#ifdef DEBUG
                    printf("Divided %d by %d, pushed %d\n", arg2, arg1, arg2/arg1);
#endif
                }
            }
        }
    } else if (strcmp("p", command) == 0){
        // print the top of the stack *without* popping it
        if (stack.Empty()){
            fprintf(stderr, "error: stack empty\n");
        } else {
#ifdef DEBUG
            printf("Top of stack: \"%d\"\n", stack.Get_element(stack.Get_top() - 1));
#else
            printf("%d\n", stack.Get_element(stack.Get_top() - 1));
#endif
        }
    } else if (strlen(command) == 0){
        // no command. EOF
    } else {
        fprintf(stderr, "error: illegal opcode (%s)\n", command);
    }
}
int main(){

    // Init stack and variable map
    Stack stack(50000);
    map<unsigned int, int> vars;

    while(!cin.eof()){

        char command[MAX_COMM_LEN];
        memset(command, 0, MAX_COMM_LEN);
        cin.getline(command, MAX_COMM_LEN);

        runOpcode(command, stack, vars);

    }
}
