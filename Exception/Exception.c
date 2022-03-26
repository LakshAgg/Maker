#include <stdlib.h>
#include "../CLI/CLI.h"
#include <stdio.h>
#include <signal.h>
#include "Exception.h"

jmp_buf *___jmp_stack;
unsigned long ___jmp_stack_height;

Exception ___exception;
void ___pop();
void ___set_handle()
{
    exception_handler = &___exception;
}

jmp_buf *____top()
{
    return ___jmp_stack + (___jmp_stack_height - 1);
}
void ___print_error()
{
    if (___exception.code != 0)
    {
        printf( S_ Bold  White _E "\n\nFatal Unhandled Exception:\n  Code => %i\n  Exception => "  S_ Red _E "%s"  S_ Bold  White _E "\n  Use catch/catchall to handle errors\n"  S_ Reset _E,
            ___exception.code, 
            ___exception.message);
    }
    else
    {
        printf( S_ Bold  White _E "\n\nFatal Unhandled Exception:\n  Exception => "  S_ Bold  Red _E "%s"  S_ Bold  White _E "\n"  S_ Reset _E,
            ___exception.message);
    }
}

void ___throw(unsigned long code, char *message)
{
    ___exception.code = code;
    ___exception.message = message;
    ___print_error();
    for (; ___jmp_stack_height > 0;)
        ___pop();
    exit(1);
}
void err_handle(int x)
{
    switch (x)
    {
    case SIGSEGV:
        ___throw_error(x, "Segmentation Fault");
        break;
    case SIGILL:
        ___throw_error(x, "illegal instruction");
        break;
    case SIGFPE:
        ___throw_error(x, "arithmetic error");
        break;
    default:
        ___throw_error(x, "Internal Error");
        break;
    }
}

void ___alloc_push_stack()
{
    ___set_handle();
    if (___jmp_stack_height <= 0)
    {
        ___jmp_stack = malloc(sizeof(jmp_buf));
        if (___jmp_stack == NULL)
            ___throw(11, "Memory error");
        ___jmp_stack_height = 1;
    }
    else
    {
        ___jmp_stack_height++;
        jmp_buf *tmp = realloc(___jmp_stack, sizeof(jmp_buf) * ___jmp_stack_height);
        if (tmp == NULL)
        {
            free(___jmp_stack);
            ___throw(11, "Memory error");
        }
        ___jmp_stack = tmp;
    }
}
void ___pop()
{
    ___set_handle();
    if (___jmp_stack_height == 0)
        ___throw(11, "Segmentation fault");

    ___jmp_stack_height--;
    if (___jmp_stack_height == 0)
    {
        free(___jmp_stack);
        ___jmp_stack = NULL;
        return;
    }
    jmp_buf *tmp = realloc(___jmp_stack, sizeof(jmp_buf) * ___jmp_stack_height);
    if (tmp == NULL)
    {
        free(___jmp_stack);
        ___throw(11, "Memory error");
    }
    ___jmp_stack = tmp;
}
void ____start_try()
{
    void * x = signal(SIGSEGV, err_handle);
    if (x != NULL)
        signal(SIGSEGV, x);

    x = signal(SIGFPE, err_handle);
    if (x != NULL)
        signal(SIGFPE, x);

    x = signal(SIGILL, err_handle);
    if (x != NULL)
        signal(SIGILL, x);
    ___set_handle();
    ___alloc_push_stack();
}
char ___catcherr()
{
    ___pop();
    return 1;
}
void ____end_try()
{
    ___set_handle();
    if (___exception.internal_code != 0)
        ___throw(___exception.code, ___exception.message);

    if (___exception.code == 0 && ___exception.internal_code == 0)
        ___pop();

    void * x = signal(SIGSEGV, NULL);
    if (x != err_handle)
        signal(SIGSEGV, x);

    x = signal(SIGFPE, NULL);
    if (x != err_handle)
        signal(SIGFPE, x);

    x = signal(SIGILL, NULL);
    if (x != err_handle)
        signal(SIGILL, x);
}
void ___throw_error(int code, char *message)
{
    ___exception.code = code;
    ___exception.internal_code = code;
    ___exception.message = message;
    ___set_handle();
    if (___jmp_stack_height <= 0)
        ___throw(___exception.code, ___exception.message);
    longjmp(*(____top()), code);
}

void ___assert(char i, char *exp, char *file_name, int line, const char func[])
{
    if (!i)
    {
        if (___jmp_stack_height <= 0)
        {
            printf(S_ Bold  White _E"\n\nFatal Assertion Error:\n   Assertion " S_ Red _E"\"%s\" "S_ White _E " has failed\n   In function %s\n   In file: %s\n   At line: %d\n" S_ Reset _E, exp, &(func[0]), file_name, line);
            abort();
        }
        else
        {
            ___throw_error(ASSERTION_ERR, "Assertion Error");
        }
    }
}