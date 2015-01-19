#include    <stdlib.h>

#include    <storage.h>
#include    <stdio.h>

Jmp *
JmpNew (void)
{
    return (Jmp *) calloc (1, sizeof (Jmp));
}

void
JmpPush (Jmp ** jmps,
         long int value)
{
    Jmp * new = JmpNew ();
    new->offset = value;
    new->next = *jmps;
    *jmps = new;
}

long int
JmpGet (Jmp * jmps)
{
    return jmps->offset;
}

void
JmpDeleteFirst (Jmp ** jmps)
{
    Jmp * tmp = *jmps;
    *jmps = (*jmps)->next;
    JmpFreeSingle (tmp);
}

void
JmpFreeSingle (Jmp * jmp)
{
    free (jmp);
}

void
JmpFreeGlobal (Jmp * jmps)
{
    Jmp * tmp = jmps;
    jmps = tmp->next;
    for (; jmps; tmp = jmps, jmps = jmps->next)
        JmpFreeSingle (tmp);
}

Stack *
StackNew (void)
{
    return (Stack *) calloc (1, sizeof (Stack));
}

void
StackPrev (Stack ** stack)
{
    if (!(*stack)->prev)
        (*stack)->prev = StackNew ();
    (*stack)->prev->next = (*stack);
    *stack = (*stack)->prev;
}

void
StackNext (Stack ** stack)
{
    if (!(*stack)->next)
        (*stack)->next = StackNew ();
    (*stack)->next->prev = (*stack);
    *stack = (*stack)->next;
}

void
StackInc (Stack * stack)
{
    stack->mem++;
}

void
StackDec (Stack * stack)
{
    stack->mem--;
}

int
StackGet (Stack * stack)
{
    return stack->mem;
}

void
StackSet (Stack * stack,
          int value)
{
    stack->mem = value;
}

void
StackFreeSingle (Stack * stack)
{
    free (stack);
}

void
StackFreeGlobal (Stack * stack)
{
    Stack * tmp;
    for (; stack->prev; stack = stack->prev) ;

    for (tmp = stack, stack = tmp->next; stack; tmp = stack, stack = stack->next)
        StackFreeSingle (stack);
}

void
StackPrint (Stack * stack)
{
	Stack * tmp = stack;
	char currentStackSpend = 0;
	int i;
	int cursorPosition=0;

	while(tmp->prev) tmp=tmp->prev;
	do {
		if(tmp==stack) currentStackSpend=1;
		i=printf("%d",tmp->mem);
		if(!currentStackSpend) cursorPosition+=i+1;
		if(tmp->next) printf(" ");
	} while((tmp=tmp->next));
	printf("\n");

	for(i=0; i<cursorPosition; i++) printf(" ");
	printf("^\n");
}
