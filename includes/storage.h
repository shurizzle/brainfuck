#ifndef _BRAINFUCK_STORAGE_H
#define _BRAINFUCK_STORAGE_H

#include    <common.h>

/* Jmps managing functions */
Jmp     *   JmpNew          (void);
void        JmpPush         (Jmp **, long int);
long int    JmpGet          (Jmp *);
void        JmpDeleteFirst  (Jmp **);
void        JmpFreeSingle   (Jmp *);
void        JmpFreeGlobal   (Jmp *);

/* Stack managing functions */
Stack   *   StackNew            (void);
void        StackPrev           (Stack **);
void        StackNext           (Stack **);
void        StackInc            (Stack *);
void        StackDec            (Stack *);
int         StackGet            (Stack *);
void        StackSet            (Stack *, int);
void        StackFreeSingle     (Stack *);
void        StackFreeGlobal     (Stack *);

#endif
