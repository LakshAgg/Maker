#ifndef Exception__H
#define Exception__H
#include <setjmp.h>

typedef struct 
{
    char *message;
    int code;
    int internal_code;
} Exception;

extern Exception *exception_handler;

void ____start_try();
void ____end_try();
jmp_buf *____top();
char ___catcherr();
void ___throw_error(int code, char *message);
void ___assert(char i, char *exp, char *file_name, int line, const char func[]);

#define try {\
    ____start_try();\
    if ((exception_handler->code = setjmp(*(____top()))) == 0)

#define catch(c) else if (exception_handler->code == c && (exception_handler->internal_code = 0) == 0 && ___catcherr())
#define catchall else if ((exception_handler->internal_code = 0) == 0 && ___catcherr())
#define throw(code, message) ___throw_error(code, message)
#define endtry ____end_try();}

#define ASSERTION_ERR 100
#define assert_e(exp) ___assert(exp, #exp, __FILE__, __LINE__, __func__)
#endif