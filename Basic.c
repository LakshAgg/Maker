#include <stdio.h>
#include "Basic.h"
#include <stdlib.h>
// its a part of CPlugins repo

typedef struct
{
    __node_free *head;
    __node_free *tail;
} __head_free;

__head_free ___free_list = {NULL, NULL};
__head_free ___empty_ctrs = {NULL, NULL};

void __free_ptr(__node_free *ptr)
{
    if (ptr == NULL) return;
    free(ptr->ptr);
    ptr->ptr = NULL;
    if (___empty_ctrs.head == NULL)
    {
        ___empty_ctrs.head = malloc(sizeof(__node_free));
        if (___empty_ctrs.head == NULL) return;
        ___empty_ctrs.tail =  ___empty_ctrs.head;
        ___empty_ctrs.head->next = NULL;
        ___empty_ctrs.head->ptr = ptr;
    }
    else
    {
        ___empty_ctrs.tail->next = malloc(sizeof(__node_free));
        if (___empty_ctrs.tail->next == NULL) return;
        ___empty_ctrs.tail = ___empty_ctrs.tail->next;
        ___empty_ctrs.tail->next = NULL;
        ___empty_ctrs.tail->ptr = ptr;
    }
}

void *add_to_be_freed(void *ptr)
{
    if (ptr == NULL) return NULL;
    if (___empty_ctrs.head != NULL)
    {
        __node_free *nf = ((__node_free *) ___empty_ctrs.head->ptr);
        nf->ptr = ptr;
        __node_free *tmp = ___empty_ctrs.head->next;
        free(___empty_ctrs.head);
        ___empty_ctrs.head = tmp;
        return nf;
    }
    if (___free_list.head == NULL)
    {
        ___free_list.head = malloc(sizeof(__node_free));
        if (___free_list.head == NULL) return NULL;
        ___free_list.tail =  ___free_list.head;
        ___free_list.head->next = NULL;
        ___free_list.head->ptr = ptr;
    }
    else
    {
        ___free_list.tail->next = malloc(sizeof(__node_free));
        if (___free_list.tail->next == NULL) return NULL;
        ___free_list.tail = ___free_list.tail->next;
        ___free_list.tail->next = NULL;
        ___free_list.tail->ptr = ptr;
    }
    return ___free_list.tail;
}

void ___cleanup_free_all()
{
    for (; ___free_list.head != NULL; )
    {
        if (___free_list.head->ptr != NULL)
            free(___free_list.head->ptr);
        __node_free *tmp = ___free_list.head->next;
        free(___free_list.head);
        ___free_list.head = tmp;
    }
    for (; ___empty_ctrs.head != NULL; )
    {
        __node_free *tmp = ___empty_ctrs.head->next;
        free(___empty_ctrs.head);
        ___empty_ctrs.head = tmp;
    }
}

#if defined (_MSC_VER) // MSVC
    #pragma section(".CRT$XCU",read)
    #define INITIALIZER_(FUNC,PREFIX) \
        static void FUNC(void); \
        __declspec(allocate(".CRT$XCU")) void (*FUNC##_)(void) = FUNC; \
        __pragma(comment(linker,"/include:" PREFIX #FUNC "_")) \
        static void FUNC(void)
    #ifdef _WIN64
        #define INITIALIZER(FUNC) INITIALIZER_(FUNC,"")
    #else
        #define INITIALIZER(FUNC) INITIALIZER_(FUNC,"_")
    #endif
#elif defined (__GNUC__) // GCC, Clang, MinGW
    #define INITIALIZER(FUNC) \
        static void FUNC(void) __attribute__((constructor)); \
        static void FUNC(void)
#else
    #error This library requires some compiler-specific features, \
           but we do not recognize this compiler/version.
#endif

INITIALIZER(setup)
{
    atexit(___cleanup_free_all);
}