#include    <stdio.h>
#include    <stdlib.h>
#include    <sys/ioctl.h>
#include    <unistd.h>
#include    <termios.h>

#include    <limits.h>
#include    <math.h>

#include    <storage.h>

#define     DEFAULT_WIDTH   80

char myget(void);
void print_debug(Stack *);

int
main(int argc,
      char ** argv) {
    Stack *stack;
    Jmp   *jmps;
    FILE  *fp;
    char   ch;

    /* Disable buffering to get instant character printing. Useful when a
     * program is stuck in a loop. */
    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc != 2) {
        printf("USAGE: %s <file>\n", argv[0]);
#ifdef BF_COMMENT_CHAR
        printf("Comment char is '%c'.\n", BF_COMMENT_CHAR);
#endif
        exit(1);
    }

    if (!(fp = fopen(argv[1], "r"))) {
        printf("File \"%s\" doesn't exist.\n", argv[1]);
        exit(1);
    }

    stack = StackNew();
    jmps  = JmpNew();

    while ((fread(&ch, 1, 1, fp)) == 1) {
        switch (ch) {
#ifdef BF_COMMENT_CHAR
        case BF_COMMENT_CHAR:
            while (fread(&ch, 1, 1, fp) == 1 && ch != '\n');
            break;
#endif
        case ',':
            StackSet(stack, myget());
            break;
        case '.':
            putchar(StackGet(stack));
            break;
        case '<':
            StackPrev(&stack);
            break;
        case '>':
            StackNext(&stack);
            break;
        case '+':
            StackInc(stack);
            break;
        case '-':
            StackDec(stack);
            break;
        case '[':
            if (StackGet(stack)) {
                JmpPush(&jmps, ftell(fp));
            }
            else {
                long nesting_level = 0;
                while (fread(&ch, 1, 1, fp) == 1 &&
                       (ch != ']' || nesting_level != 0)) {
                    if (ch == '[') {
                        nesting_level++;
                    }
                    else if (ch == ']') {
                        nesting_level--;
                    }
                }
            }
            break;
        case ']':
            if (StackGet(stack)) {
                fseek(fp, JmpGet(jmps), SEEK_SET);
            }
            else {
                JmpDeleteFirst(&jmps);
            }
            break;
        case '^':
            print_debug(stack);
        }
    }

    fclose(fp);
    JmpFreeGlobal(jmps);
    StackFreeGlobal(stack);

    return 0;
}

char
myget (void) {
    struct termios t, old;
    char ch;

    tcgetattr(fileno(stdin), &t);
    old = t;
    cfmakeraw(&t);
    tcsetattr(fileno(stdin), 0, &t);
    ch = getchar();
    tcsetattr(fileno(stdin), 0, &old);

    return ch;
}

unsigned short
get_width() {
    struct winsize w;
    if (!isatty(fileno(stdout))) {
        return DEFAULT_WIDTH;
    }

    ioctl(fileno(stdout), TIOCGWINSZ, &w);

    return w.ws_col < 1 ? DEFAULT_WIDTH : w.ws_col;
}

void
print_debug(Stack *current) {
    Stack *stack = current;
    unsigned int el = (unsigned int) log10(UINT_MAX) + 1;
    unsigned int epl = ((unsigned int) get_width()) / (el + 4);
    unsigned int ce = 0;
    const char *template = (const char *) calloc(log10(el) + 8, sizeof(char));
    sprintf((char *) template, "0x%%-%ux", el);

    epl = epl < 1 ? 1 : epl;
    while (stack->prev != NULL) {
        stack = stack->prev;
    }

    do {
        putchar(stack == current ? '[' : ' ');
        printf(template, current->mem);
        putchar(stack == current ? ']' : ' ');
        
        if (++ce == epl) {
            ce = 0;
            putchar(10);
        }
    } while ((stack = stack->next) != NULL);

    if (ce != 0) {
        putchar(10);
    }
}
