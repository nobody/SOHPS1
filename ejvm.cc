#include<stdio.h>
#include<iostream>
#include<sstream>
#include<string.h>
#include<string>
#include<vector>
#include<map>

#include "stack.h"

using namespace std;


enum Comm { C_INVALID, C_CONST, C_POP, C_STORE, C_LOAD, C_ADD, C_SUB, C_MUL, C_DIV, C_P,
            C_INC, C_GOTO, C_IFEQ, C_IFNE, C_IFLT, C_IFGT, 
            C_INVOKE, C_RETURN, C_FETCH
          };

struct symEntry{
    int lineNum;
    int limit;

    symEntry(int ln, int lim = -1) : lineNum(ln), limit(lim) {}
};

struct Opcode{
    Comm type;
    string label;
    int intArg1;
    int intArg2;
    string strArg;

    Opcode() : type(C_INVALID), label(), intArg1(0), intArg2(0), strArg() {}
};



void execOpcode(int &PC, Stack &stack, vector<Opcode*> &program, map<unsigned int, int> &vars, map<string, symEntry*> &symbols){

    Opcode op = *(program[PC]);
    PC++;
    switch(op.type) {
        case C_CONST:
            if (stack.Full()){
                fprintf(stderr, "error: stack full\n");
            } else {
                stack.Push(op.intArg1);
#ifdef DEBUG
                printf("Pushed constant \"%d\" onto stack\n", arg);
#endif
            }
            break;
        case C_POP:
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
            } else {
                int popped = stack.Pop();
#ifdef DEBUG
                printf("Popped \"%d\" from the stack\n", popped);
#endif
            }
            break;
        case C_STORE:
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
            } else {
                int val = stack.Pop();
                vars[op.intArg1] = val;
#ifdef DEBUG
                printf("Set var %d to value %d\n", op.intArg1, val);
#endif
            }
            break;
        case C_LOAD:
            if (stack.Full()){
                fprintf(stderr, "error: stack full\n");
            } else {
                stack.Push(vars[op.intArg1]);
#ifdef DEBUG
                printf("Pushed var %d (value %d) onto stack\n", op.intArg1, vars[op.intArg1]);
#endif
            }
            break;
        case C_ADD:
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
            break;
        case C_SUB:
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
            break;
        case C_MUL:
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
            break;
        case C_DIV:
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
            break;
        case C_P:
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
            } else {
#ifdef DEBUG
                printf("Top of stack: \"%d\"\n", stack.Get_element(stack.Get_top() - 1));
#else
                printf("%d\n", stack.Get_element(stack.Get_top() - 1));
#endif
            }
            break;
        case C_INC:
            // increment local variable arg1 by arg2
            vars[op.intArg1] += op.intArg2;
#ifdef DEBUG
            printf("Increment var %d by %d. New Val %d\n", op.intArg1, op.intArg2, vars[op.intArg1]);
#endif
            break;
        case C_GOTO:
            PC = symbols[op.strArg]->lineNum;

#ifdef DEBUG
            printf("Unconditional jump to label %s (line %d)\n", op.strArg.c_str(), symbols[op.strArg]->lineNum);
#endif
            break;
        case C_IFEQ:
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
            } else {
                int popped = stack.Pop();
                if (popped == 0){
                    PC = symbols[op.strArg]->lineNum;
#ifdef DEBUG
            printf("[IFEQ] Top of stack was %d. Jump to label %s (line %d)\n", popped, op.strArg.c_str(), symbols[op.strArg]->lineNum);
#endif
                } else {
#ifdef DEBUG
            printf("[IFEQ] Top of stack was %d. Do NOT jump to label %s (line %d)\n", popped, op.strArg.c_str(), symbols[op.strArg]->lineNum);
#endif
                }
            }
            break;
        case C_IFNE:
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
            } else {
                int popped = stack.Pop();
                if (popped != 0){
                    PC = symbols[op.strArg]->lineNum;
#ifdef DEBUG
            printf("[IFNE] Top of stack was %d. Jump to label %s (line %d)\n", popped, op.strArg.c_str(), symbols[op.strArg]->lineNum);
#endif
                } else {
#ifdef DEBUG
            printf("[IFNE] Top of stack was %d. Do NOT jump to label %s (line %d)\n", popped, op.strArg.c_str(), symbols[op.strArg]->lineNum);
#endif
                }
            }
            break;
        case C_IFLT:
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
            } else {
                int pop1 = stack.Pop();
                if (stack.Empty()){
                    stack.Push(pop1);
                    fprintf(stderr, "error: stack empty\n");
                } else {
                    int pop2 = stack.Pop();
                    if (pop2 < pop1){
                        PC = symbols[op.strArg]->lineNum;
#ifdef DEBUG
                printf("[IFLT] Top of stack was %d. Jump to label %s (line %d)\n", popped, op.strArg.c_str(), symbols[op.strArg]->lineNum);
#endif
                    } else {
#ifdef DEBUG
                printf("[IFLT] Top of stack was %d. Do NOT jump to label %s (line %d)\n", popped, op.strArg.c_str(), symbols[op.strArg]->lineNum);
#endif
                    }
                }
            }
            break;
        case C_IFGT:
            if (stack.Empty()){
                fprintf(stderr, "error: stack empty\n");
            } else {
                int pop1 = stack.Pop();
                if (stack.Empty()){
                    stack.Push(pop1);
                    fprintf(stderr, "error: stack empty\n");
                } else {
                    int pop2 = stack.Pop();
                    if (pop2 > pop1){
                        PC = symbols[op.strArg]->lineNum;
#ifdef DEBUG
                printf("[IFLT] Top of stack was %d. Jump to label %s (line %d)\n", popped, op.strArg.c_str(), symbols[op.strArg]->lineNum);
#endif
                    } else {
#ifdef DEBUG
                printf("[IFLT] Top of stack was %d. Do NOT jump to label %s (line %d)\n", popped, op.strArg.c_str(), symbols[op.strArg]->lineNum);
#endif
                    }
                }
            }

        default:
            printf("Executing unknown opcode %d\n", op.type);
    }            
}


bool addline(int idx, char* line, vector<Opcode*> &program, map<string, symEntry*> &symbols){

    Opcode* opcode = new Opcode;

    stringstream ss(line);
    char command[50];
    memset(command, 0, 50);

    ss >> command;

    // check if this is a label instead of a command
    // TODO: Check that it is OK to require a space between "label:" and the opcode
    char* colon = strstr(command, ":");
    if (colon != NULL){
        // get the label
        opcode->label = command;

        opcode->label = opcode->label.substr(0, opcode->label.find_first_of(':'));

        printf("Found label %s\n", opcode->label.c_str());

        printf("Adding label #1\n");
        symbols[opcode->label] = new symEntry(idx);
        printf("Added label to symbol table with value %d\n", symbols[opcode->label]->lineNum);

        memset(command, 0, 50);
        ss >> command;
    }

    // Is this a method declaration?
    if (strcmp(".method", command) == 0){
        
        // get name
        string name;
        ss >> name;
        if (ss.fail()){
            fprintf(stderr, "Getting method name failed\nInvalid method declaration -- abort.\n");
            delete opcode;
            return false;
        } else {
            // read next line and get limit
            memset(line, 0, 50);
            cin.getline(line, 50);
            stringstream sslimit(line);

            memset(command, 0, 50);
            sslimit >> command;

            if (strcmp(".limit", command) == 0){
                // grab the limit
                int limit = 0;
                
                sslimit >> limit;
                if (sslimit.fail()){
                    char arg[25];
                    memset(arg, 0, 25);
                    sslimit >> arg;

                    fprintf(stderr, "Getting variable limit failed (argument: %s)\nInvalid method declaration -- abort.\n", arg);
                    delete opcode;
                    return false;

                } else {
                    // add to symbol table
                    symbols[name] = new symEntry(idx, limit);
                    
                    delete opcode;
                    return true;
                }
            } else {
                fprintf(stderr, "Malformed method declaration -- no limit line.\n");
                delete opcode;
                return false;
            }
        }
    }

    if (strcmp("const", command) == 0){
        // set constant arg in opcode

        int arg = 0;
        ss >> arg;
        if (ss.fail()){
            ss.clear();
            char arg[25];
            memset(arg, 0, 25);
            ss >> arg;

            printf("Error reading augument: \"%s\".  Not a number.\n", arg);
            goto MALFORMED;
        } else {
            opcode->type = C_CONST;
            opcode->intArg1 = arg;
        }
    } else if (strcmp("pop", command) == 0){
        opcode->type = C_POP;

    } else if (strcmp("store", command) == 0){
        // set var name arg in opcode

        int arg = 0;
        ss >> arg;
        if (ss.fail()){
            ss.clear();
            char arg[25];
            memset(arg, 0, 25);
            ss >> arg;
            printf("Error reading augument: \"%s\".  Not a number.\n", arg);
            goto MALFORMED;
        } else {
            if (arg < 0) {
                fprintf(stderr, "error: variable index should be non-negative\n");
                goto MALFORMED;
            } else {
                opcode->type = C_STORE;
                opcode->intArg1 = arg;
            }
        }

    } else if (strcmp("load", command) == 0){
        // set var name arg in opcode

        int arg = 0;
        ss >> arg;
        if (ss.fail()){
            ss.clear();
            char arg[25];
            memset(arg, 0, 25);
            ss >> arg;
            printf("Error reading augument: \"%s\".  Not a number.\n", arg);
            goto MALFORMED;
        } else {
            if (arg < 0){
                fprintf(stderr, "error: variable index should be non-negative\n");
                goto MALFORMED;
            } else {
                opcode->type = C_LOAD;
                opcode->intArg1 = arg;
            }
        }

    } else if (strcmp("add", command) == 0){
        opcode->type = C_ADD;

    } else if (strcmp("sub", command) == 0){
        opcode->type = C_SUB;

    } else if (strcmp("mul", command) == 0){
        opcode->type = C_MUL;

    } else if (strcmp("div", command) == 0){
        opcode->type = C_DIV;

    } else if (strcmp("p", command) == 0){
        opcode->type = C_P;

    } else if (strcmp("inc", command) == 0){
        // get 2 arguments out
        
        int arg1 = 0;
        ss >> arg1;
        if (ss.fail()){
            ss.clear();
            char arg[25];
            memset(arg, 0, 25);
            ss >> arg;
            printf("Error reading augument1: \"%s\".  Not a number.\n", arg);
            goto MALFORMED;
        } else {
            if (arg1 < 0){
                fprintf(stderr, "error: variable index should be non-negative\n");
                goto MALFORMED;
            } else {
                int arg2 = 0;
                ss >> arg2;
                if (ss.fail()){
                    ss.clear();
                    char arg[25];
                    memset(arg, 0, 25);
                    ss >> arg;
                    printf("Error reading augument2: \"%s\".  Not a number.\n", arg);
                    goto MALFORMED;
                } else {
                    opcode->type = C_INC;
                    opcode->intArg1 = arg1;
                    opcode->intArg2 = arg2;
                }
            }
        }

    } else if (strcmp("goto", command) == 0){
        // get label argument

        ss >> opcode->strArg;
        if (ss.fail()){
            fprintf(stderr, "Getting jump destination failed\n");
            goto MALFORMED;
        } else {
            opcode->type = C_GOTO;
        }
    } else if (strcmp("ifeq", command) == 0){
        // get label argument
        
        ss >> opcode->strArg;
        if (ss.fail()){
            fprintf(stderr, "Getting jump destination failed\n");
            goto MALFORMED;
        } else {
            opcode->type = C_IFEQ;
        }

    } else if (strcmp("ifne", command) == 0){
        // get label argument
        
        ss >> opcode->strArg;
        if (ss.fail()){
            fprintf(stderr, "Getting jump destination failed\n");
            goto MALFORMED;
        } else {
            opcode->type = C_IFNE;
        }

    } else if (strcmp("iflt", command) == 0){
        // get label argument
        
        ss >> opcode->strArg;
        if (ss.fail()){
            fprintf(stderr, "Getting jump destination failed\n");
            goto MALFORMED;
        } else {
            opcode->type = C_IFLT;
        }

    } else if (strcmp("ifgt", command) == 0){
        // get label argument
        
        ss >> opcode->strArg;
        if (ss.fail()){
            fprintf(stderr, "Getting jump destination failed\n");
            goto MALFORMED;
        } else {
            opcode->type = C_IFGT;
        }

    } else if (strcmp("invoke", command) == 0){
        // get method name

        ss >> opcode->strArg;
        if (ss.fail()){
            fprintf(stderr, "Getting called method name failed\n");
            goto MALFORMED;
        } else {
            opcode->type = C_INVOKE;
        }

    } else if (strcmp("return", command) == 0){
        opcode->type = C_RETURN;

    } else if (strcmp("fetch", command) == 0){
        // fetch n x (n is the local variable, x is the offset in the stack)

        int arg1 = 0;
        ss >> arg1;
        if (ss.fail()){
            ss.clear();
            char arg[25];
            memset(arg, 0, 25);
            ss >> arg;
            printf("Error reading augument1: \"%s\".  Not a number.\n", arg);
            goto MALFORMED;
        } else {
            if (arg1 < 0){
                fprintf(stderr, "error: variable index should be non-negative\n");
                goto MALFORMED;
            } else {
                int arg2 = 0;
                ss >> arg2;
                if (ss.fail()){
                    ss.clear();
                    char arg[25];
                    memset(arg, 0, 25);
                    ss >> arg;
                    printf("Error reading augument2: \"%s\".  Not a number.\n", arg);
                    goto MALFORMED;
                } else {
                    opcode->type = C_FETCH;
                    opcode->intArg1 = arg1;
                    opcode->intArg2 = arg2;
                }
            }
        }

    } else if (strlen(command) == 0){
        // no command. EOF
    } else {
        fprintf(stderr, "error: illegal opcode (%s)\n", command);
    }

    if (opcode->type != C_INVALID){
        // add to program
        program.push_back(opcode);

    } else {
        // if there was a label here, 
        if (opcode->label.length() > 0){
            goto MALFORMED;
        }
        delete opcode;
    }

    return true;

MALFORMED:
    delete opcode;
    fprintf(stderr, "Malformed Instruction -- Aborting.\n");
    return false;
}

int main(){

    // Init stack and variable map
    Stack stack(100);
    map<unsigned int, int> vars;
    vector<Opcode*> program;
    map<string, symEntry*> symbols;

    while(!cin.eof()){

        char* command = new char[50];
        memset(command, 0, 50);
        cin.getline(command, 50);

        // add line to stored program
        if (addline(program.size(), command, program, symbols) == false)
            return 1;
    }

    vector<Opcode*>::iterator it;
    int ctr = 0;

    for (it = program.begin(); it < program.end(); it++) {
        // check if there is a method that starts here

        map<string, symEntry*>::iterator symit;
        for (symit = symbols.begin(); symit != symbols.end(); symit++){
            if ((*symit).second->lineNum == ctr && (*symit).second->limit >= 0){
                printf("                .method %s\n", (*symit).first.c_str());
                printf("                .limit %d\n", (*symit).second->limit);
                break;
            }
        }

        if ((*it)->label.length() > 0){
            printf("%02d %10s:  ", ctr, (*it)->label.c_str());
        } else {
            printf("%02d              ", ctr);
        }

        switch((*it)->type){
            case C_CONST:
                printf("const %d\n", (*it)->intArg1);
                break;
            case C_POP:
                printf("pop \n");
                break;
            case C_STORE:
                printf("store %d\n", (*it)->intArg1);
                break;
            case C_LOAD:
                printf("load %d\n", (*it)->intArg1);
                break;
            case C_ADD:
                printf("add \n");
                break;
            case C_SUB:
                printf("sub \n");
                break;
            case C_MUL:
                printf("mul \n");
                break;
            case C_DIV:
                printf("div \n");
                break;
            case C_P:
                printf("p \n");
                break;


            case C_INC:
                printf("inc %d %d\n", (*it)->intArg1, (*it)->intArg2);
                break;
            case C_GOTO:
                printf("goto %s\n", (*it)->strArg.c_str());
                break;
            case C_IFEQ:
                printf("ifeq %s\n", (*it)->strArg.c_str());
                break;
            case C_IFNE:
                printf("ifne %s\n", (*it)->strArg.c_str());
                break;
            case C_IFLT:
                printf("iflt %s\n", (*it)->strArg.c_str());
                break;
            case C_IFGT:
                printf("ifgt %s\n", (*it)->strArg.c_str());
                break;


            case C_INVOKE:
                printf("invoke %s\n", (*it)->strArg.c_str());
                break;
            case C_RETURN:
                printf("return \n\n");
                break;
            case C_FETCH:
                printf("fetch %d %d\n", (*it)->intArg1, (*it)->intArg2);
                break;
        }

        ctr++;
    }

    printf("\n\nSymbol Table\n");

    map<string, symEntry*>::iterator symit;

    for (symit = symbols.begin(); symit != symbols.end(); symit++){
        printf("%10s:  %3d  %4d\n", (*symit).first.c_str(), (*symit).second->lineNum, (*symit).second->limit);
    }


    printf("\n\n\nBeginning Execution. Output below:\n");

    int PC = 0;

    // Execute program
    while(PC < program.size() && PC >= 0) { 
        // While we are still in the program...

        execOpcode(PC, stack, program, vars, symbols);
    }
    printf("stack size: %d\n", stack.Get_top());
}
