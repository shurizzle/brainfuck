#include    <stdio.h>
#include    <stdlib.h>
#include    <termios.h>

#include    <storage.h>

char myget (void);

int
main (int argc,
      char ** argv)
{
    Stack * stack;
    Jmp * jmps;
    FILE * fp;
    char   ch;

    /* Disable buffering to get instant character printing. Useful when a
     * program is stuck in a loop. */
    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc != 2)
    {
        printf ("USAGE: %s <file>\n", argv[0]);
#ifdef BF_COMMENT_CHAR
        printf ("Comment char is '%c'.\n", BF_COMMENT_CHAR);
#endif
        exit (1);
    }

    if (!(fp = fopen (argv[1], "r")))
    {
        printf ("File \"%s\" doesn't exist.\n", argv[1]);
        exit (1);
    }

    stack = StackNew ();
    jmps  = JmpNew ();

    while ((fread (&ch, 1, 1, fp)) == 1)
    {
        switch (ch)
        {
#ifdef BF_COMMENT_CHAR
        case BF_COMMENT_CHAR:
            while (fread (&ch, 1, 1, fp) == 1 && ch != '\n');
            break;
#endif
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
            if (StackGet (stack))
                JmpPush (&jmps, ftell (fp));
            else
            {
                long nesting_level = 0;
                while (fread (&ch, 1, 1, fp) == 1 &&
                       (ch != ']' || nesting_level != 0))
                {
                    if (ch == '[')
                        nesting_level++;
                    else if (ch == ']')
                        nesting_level--;
                }
            }
            break;
        case ']':
            if (StackGet (stack))
                fseek (fp, JmpGet (jmps), SEEK_SET);
            else
                JmpDeleteFirst (&jmps);
            break;
		case '#':
			StackPrint(stack);
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
    struct termios t, old;
    char ch;

    tcgetattr (fileno(stdin), &t);
    old = t;
    cfmakeraw (&t);
    tcsetattr (fileno(stdin), 0, &t);
    ch = getchar();
    tcsetattr (fileno(stdin), 0, &old);

    return ch;
}
