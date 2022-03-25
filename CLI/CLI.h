#ifndef __CLI_Header
#define __CLI_Header

#include <stdbool.h>

#define S_ "\033["
#define Black "30"
#define Red "31"
#define Green "32"
#define Yellow "33"
#define Blue "34"
#define Magenta "35"
#define Bold "1;"
#define Underline "4;"
#define Cyan "36"
#define White "37"
#define Reset "0"
#define Invert "7;"
#define _E "m"

// typedef struct
// {
//     char start;
//     char end;
//     char fill;
//     char *color;
//     char empty;
//     unsigned long progress, max;
// } Bar;


// // creates a progress bar
// Bar *create_bar();

// void update_bar(Bar *bar);

bool ask_bool(char *ques);

// case insensitive
bool matches_ans(char *ques, char *expected);
#endif