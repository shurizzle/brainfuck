#ifndef _BRAINFUCK_COMMON_H
#define _BRAINFUCK_COMMON_H

typedef struct Stack Stack;
typedef struct Jmp Jmp;

struct Stack {
    unsigned int  mem;
    Stack   *     prev;
    Stack   *     next;
};

struct Jmp {
    long int      offset;
    Jmp     *     next;
};

#endif
