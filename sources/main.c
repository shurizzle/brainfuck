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

    stack = StackNew ();
    jmps  = JmpNew ();

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
  struct termios t, old;
  char ch;

  tcgetattr (stdin->_fileno, &t);
  old = t;
  cfmakeraw (&t);
  tcsetattr (stdin->_fileno, 0, &t);
  ch = getchar();
  tcsetattr (stdin->_fileno, 0, &old);

  return ch;
}
