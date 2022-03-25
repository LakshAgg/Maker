#include "CLI.h"
#include <stdio.h>
#include "../Input/Input.h"
#include <string.h>

// Bar *create_bar()
// {
    
// }

bool ask_bool(char *ques)
{
    while (true)
    {
        printf("%s", ques);

        char *input = get_string("");

        for (int i = 0; input[i]; i++)
        {
            if (input[i] >= 'A' && input[i] <= 'Z')
                input[i] += 'a' - 'A';
        }

        if (strcmp(input, "yes") == 0 || (input[0] == 'y' && input[1] == 0)) return true;
        else if (strcmp(input, "no") == 0 || (input[0] == 'n' && input[1] == 0)) return false;
    }
}

bool matches_ans(char *ques, char *expected)
{
    printf("%s", ques);
    char *input = get_string("");
    for (int i = 0; input[i]; i++)
    {
        if (input[i] >= 'A' && input[i] <= 'Z')
            input[i] += 'a' - 'A';
    }
    for (int i = 0; expected[i]; i++)
        if (expected[i] >= 'A' && expected[i] <= 'Z')
            expected[i] += 'a' - 'A';
    return strcmp(input, expected) == 0;
}