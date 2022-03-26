#ifndef __BASIC__Header
#define __BASIC__Header
#include <stdbool.h>

typedef struct __s_node_free
{
    void *ptr;
    struct __s_node_free *next;
} __node_free;

// adds a pointer to be freed at the end of the program
// returns NULL if unsuccessful
void *add_to_be_freed(void *ptr);

// frees a pointer allocated using malloc
// takes in pointer to the pointer to be freed
void __free_ptr(__node_free *ptr);

void ___cleanup_free_all();
#endif