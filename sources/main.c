#include    <stdio.h>
#include    <stdlib.h>

#include    <storage.h>

char myget (void);

int
main (int argc,
      char ** argv)
{
    Stack * stack = StackNew ();
    Jmp * jmps = JmpNew ();
    FILE * fp;
    char   ch;

    if (argc != 2)
    {
        printf ("USAGE: %s <file>\n", argv[0]);
        exit (1);
    }

    if (!(fp = fopen (argv[1], "r")))
    {
        printf ("File \"%s\" doesn't exist.\n", argv[1]);
        exit (1);
    }

    while ((fread (&ch, 1, 1, fp)) == 1)
    {
        switch (ch)
        {
            case ',':
                StackSet (stack, myget ());
            break;
            case '.':
                putchar (StackGet (stack));
            break;
            case '<':
                StackPrev (&stack);
            break;
            case '>':
                StackNext (&stack);
            break;
            case '+':
                StackInc (stack);
            break;
            case '-':
                StackDec (stack);
            break;
            case '[':
                JmpPush (&jmps, ftell (fp));
            break;
            case ']':
                if (StackGet (stack))
                    fseek (fp, JmpGet (jmps), SEEK_SET);
                else
                    JmpDeleteFirst (&jmps);
            break;
        }
    }

    fclose (fp);
    JmpFreeGlobal (jmps);
    StackFreeGlobal (stack);

    return 0;
}

char
myget (void)
{
    char ch[2] = {0};

    while (!ch[0])
        while ((ch[1] = getchar ()) != 10)
            ch[0] = ch[1];

    return ch[0];
}
