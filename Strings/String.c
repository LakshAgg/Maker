#include "String.h"
#include <stdio.h>
#include "../Basic.h"
#include <string.h>
#include <stdlib.h>

struct S_Str
{
    // The main value
    char *value;
    // The length of the string
    unsigned long length;
    // end [null] character of the string
    char *end;
    void *ctr, *v_ctr;
};

string new_string()
{
    string rv = malloc(sizeof(_string));
    if (rv == NULL)
        return NULL;
    rv->ctr = add_to_be_freed(rv);
    if (!rv->ctr)
    {
        free(rv);
        return NULL;
    }

    rv->length = 0;
    rv->value = malloc(1);
    if (rv->value == NULL)
    {
        __free_ptr(rv->ctr);
        return NULL;
    }
    rv->v_ctr = add_to_be_freed(rv->value);
    if (!rv->v_ctr)
    {
        free(rv->value);
        __free_ptr(rv->ctr);
        return NULL;
    }
    rv->value[0] = 0;
    rv->end = rv->value;
    return rv;
}
string arr_to_str(char *arr)
{
    if (!arr)
        return NULL;

    string rv = new_string();
    if (rv == NULL)
        return NULL;

    unsigned long length = strlen(arr);
    __free_ptr(rv->v_ctr);

    rv->value = malloc(length + 1);
    if (rv->value == NULL)
    {
        __free_ptr(rv->ctr);
        return NULL;
    }

    rv->v_ctr = add_to_be_freed(rv->value);
    if (!rv->v_ctr)
    {
        free(rv->value);
        __free_ptr(rv->ctr);
        return NULL;
    }

    memcpy(rv->value, arr, length + 1);
    rv->length = length;
    rv->end = &(rv->value[length]);
    return rv;
}
bool assign_str(string s, char *v)
{
    if (s == NULL || v == NULL)
        return false;

    unsigned long length = strlen(v);

    __free_ptr(s->v_ctr);

    s->value = malloc(length + 1);

    if (s->value == NULL)
        return false;

    s->v_ctr = add_to_be_freed(s->value);
    if (!s->v_ctr)
    {
        free(s->value);
        return false;
    }

    s->length = length;
    s->end = &(s->value[length]);
    memcpy(s->value, v, length + 1);
    return true;
}
string concat_str_str(string a, string b)
{
    if (a == NULL || b == NULL)
        return NULL;

    string rv = new_string();
    if (rv == NULL)
        return NULL;

    __free_ptr(rv->v_ctr);
    rv->length = a->length + b->length;
    rv->value = malloc(rv->length + 1);

    if (rv->value == NULL)
    {
        __free_ptr(rv->ctr);
        return NULL;
    }
    rv->v_ctr = add_to_be_freed(rv->value);
    if (!rv->v_ctr)
    {
        free(rv->value);
        __free_ptr(rv->ctr);
        return NULL;
    }

    memcpy(rv->value, a->value, a->length);
    memcpy(&(rv->value[a->length]), b->value, b->length + 1);
    rv->end = &(rv->value[rv->length]);
    return rv;
}
bool assign_concat_str_str(string a, string b)
{
    if (a == NULL || b == NULL)
        return false;

    char *temp = malloc(a->length + b->length + 1);
    if (temp == NULL)
        return false;

    memcpy(temp, a->value, a->length);
    memcpy(temp + a->length, b->value, b->length + 1);

    __free_ptr(a->v_ctr);
    a->v_ctr = add_to_be_freed(temp);
    if (!a->v_ctr)
    {
        free(temp);
        return false;
    }
    a->value = temp;
    a->length += b->length;
    a->end = a->value + a->length;
    return true;
}
string concat_str_arr(string a, char *b)
{
    unsigned long b_len = strlen(b);
    unsigned long length = a->length + b_len;
    string rv = new_string();
    if (rv == NULL)
        return NULL;
    __free_ptr(rv->v_ctr);
    rv->length = length;

    rv->value = malloc(rv->length + 1);
    if (rv->value == NULL)
    {
        __free_ptr(rv->ctr);
        return NULL;
    }
    rv->v_ctr = add_to_be_freed(rv->value);
    if (!rv->v_ctr)
    {
        free(rv->value);
        __free_ptr(rv->ctr);
        return NULL;
    }

    memcpy(rv->value, a->value, a->length);
    memcpy(&(rv->value[a->length]), b, b_len + 1);
    rv->end = &(rv->value[rv->length]);
    return rv;
}
bool assign_concat_str_arr(string a, char *b)
{
    if (a == NULL || b == NULL)
        return false;

    unsigned long b_length = strlen(b);

    char *temp = malloc(a->length + b_length + 1);
    if (temp == NULL)
        return false;

    memcpy(temp, a->value, a->length);
    memcpy(temp + a->length, b, b_length + 1);

    __free_ptr(a->v_ctr);
    a->v_ctr = add_to_be_freed(temp);
    if (!a->v_ctr)
    {
        free(temp);
        return false;
    }
    a->value = temp;
    a->length += b_length;
    a->end = a->value + a->length;
    return true;
}
string concat_arr_arr(char *a, char *b)
{
    unsigned long b_len = strlen(b), a_len = strlen(a);
    unsigned long length = a_len + b_len;
    string rv = new_string();
    if (rv == NULL)
        return NULL;
    __free_ptr(rv->v_ctr);
    rv->length = length;

    rv->value = malloc(rv->length + 1);
    if (rv->value == NULL)
    {
        __free_ptr(rv->ctr);
        return NULL;
    }
    rv->v_ctr = add_to_be_freed(rv->value);
    if (!rv->v_ctr)
    {
        free(rv->value);
        __free_ptr(rv->ctr);
        return NULL;
    }

    memcpy(rv->value, a, a_len);
    memcpy(&(rv->value[a_len]), b, b_len + 1);
    rv->end = &(rv->value[rv->length]);
    return rv;
}
string clone_str(string v)
{
    return concat_str_arr(v, "");
}
bool clone_to(string to, string from)
{
    to->length = from->length;
    __free_ptr(to->v_ctr);
    to->value = malloc(to->length + 1);
    if (to->value == NULL)
        return false;
    to->v_ctr = add_to_be_freed(to->value);
    if (!to->v_ctr)
    {
        free(to->value);
        return false;
    }
    memcpy(to->value, from->value, to->length + 1);
    to->end = &(to->value[to->length]);
    return true;
}
bool contains_str(string a, string b)
{
    if (a->length < b->length)
        return false;
    for (unsigned long x = 0, l = a->length - b->length + 1; x < l; x++)
    {
        unsigned long y = 0, m = b->length;
        for (; y < m; y++)
        {
            if (a->value[x + y] != b->value[y])
                break;
        }
        if (y == m)
            return true;
    }
    return false;
}
bool contains_arr(string a, char *b)
{
    unsigned long b_len = strlen(b);
    if (a->length < b_len)
        return false;
    for (unsigned long x = 0, l = a->length - b_len + 1; x < l; x++)
    {
        unsigned long y = 0, m = b_len;
        for (; y < m; y++)
        {
            if (a->value[x + y] != b[y])
                break;
        }
        if (y == m)
            return true;
    }
    return false;
}
bool ends_with_str(string a, string b)
{
    if (a->length < b->length)
        return false;
    unsigned long tmp = a->length - b->length;
    for (unsigned long x = 0; x < b->length; x++)
    {
        if (a->value[tmp + x] != b->value[x])
            return false;
    }
    return true;
}
bool ends_with_arr(string a, char *b)
{
    unsigned long b_len = strlen(b);
    if (a->length < b_len)
        return false;
    unsigned long tmp = a->length - b_len;
    for (unsigned long x = 0; x < b_len; x++)
    {
        if (a->value[tmp + x] != b[x])
            return false;
    }
    return true;
}
bool starts_with_str(string a, string b)
{
    if (a->length < b->length)
        return false;
    for (unsigned long x = 0; x < b->length; x++)
    {
        if (a->value[x] != b->value[x])
            return false;
    }
    return true;
}
bool starts_with_arr(string a, char *b)
{
    unsigned long b_len = strlen(b);
    if (a->length < b_len)
        return false;
    for (unsigned long x = 0; x < b_len; x++)
    {
        if (a->value[x] != b[x])
            return false;
    }
    return true;
}
string insert_str(string a, unsigned long index, string b)
{
    if (a->length < index)
        return NULL;
    string new = new_string();
    __free_ptr(new->v_ctr);
    if (new == NULL)
        return NULL;

    new->length = a->length + b->length;
    new->value = malloc(new->length + 1);
    if (new->value == NULL)
    {
        __free_ptr(new->ctr);
        return NULL;
    }
    new->v_ctr = add_to_be_freed(new->value);
    if (!new->v_ctr)
    {
        free(new->value);
        __free_ptr(new->ctr);
        return NULL;
    }
    memcpy(new->value, a->value, index);
    memcpy(&(new->value[index]), b->value, b->length);
    memcpy(&(new->value[index + b->length]), &(a->value[index]), a->length - index + 1);
    new->end = new->value + new->length;
    return new;
}
bool assign_insert_str(string a, unsigned long index, string b)
{
    if (a->length < index)
        return NULL;
    char *temp = malloc(a->length + b->length + 1);
    if (temp == NULL)
        return false;

    memcpy(temp, a->value, index);
    memcpy(temp + index, b->value, b->length);
    memcpy(temp + index + b->length, a->value + index, a->length - index + 1);
    __free_ptr(a->v_ctr);
    a->value = temp;
    a->v_ctr = add_to_be_freed(a->value);
    if (!a->v_ctr)
    {
        free(a->value);
        return false;
    }
    a->length = a->length + b->length;
    a->end = a->value + a->length;
    return true;
}
string insert_arr(string a, unsigned long index, char *b)
{
    if (a->length < index)
        return NULL;
    string new = new_string();
    __free_ptr(new->v_ctr);
    if (new == NULL)
        return NULL;

    unsigned long b_len = strlen(b);

    new->length = a->length + b_len;
    new->value = malloc(new->length + 1);
    if (new->value == NULL)
    {
        __free_ptr(new->ctr);
        return NULL;
    }

    new->v_ctr = add_to_be_freed(new->value);
    if (!new->v_ctr)
    {
        free(new->value);
        __free_ptr(new->ctr);
        return NULL;
    }

    memcpy(new->value, a->value, index);
    memcpy(&(new->value[index]), b, b_len);
    memcpy(&(new->value[index + b_len]), &(a->value[index]), a->length - index + 1);
    new->end = new->value + new->length;
    return new;
}
bool assign_insert_arr(string a, unsigned long index, char *b)
{
    if (a->length < index)
        return NULL;
    unsigned b_length = strlen(b);
    char *temp = malloc(a->length + b_length + 1);
    if (temp == NULL)
        return false;

    memcpy(temp, a->value, index);
    memcpy(temp + index, b, b_length);
    memcpy(temp + index + b_length, a->value + index, a->length - index + 1);
    __free_ptr(a->v_ctr);
    a->value = temp;
    a->v_ctr = add_to_be_freed(a->value);
    if (!a->v_ctr)
    {
        free(a->value);
        return false;
    }
    a->length = a->length + b_length;
    a->end = a->value + a->length;
    return true;
}
string str_trim_start(string a)
{
    if (a == NULL)
        return NULL;

    if (a->value[0] != ' ')
        return clone_str(a);
    unsigned long f = 0;
    while (a->value[f] == ' ')
    {
        f++;
    }

    size_t len = a->length - f;
    char *n = malloc(len + 1);
    if (n == NULL)
        return NULL;

    memcpy(n, &(a->value[f]), len + 1);

    string rv = new_string();
    if (rv == NULL)
        return NULL;
    __free_ptr(rv->v_ctr);
    rv->v_ctr = add_to_be_freed(n);
    if (!rv->v_ctr)
    {
        free(n);
        __free_ptr(rv->ctr);
        return NULL;
    }

    rv->value = n;
    rv->length = len;
    rv->end = n + rv->length;
    return rv;
}
bool assign_str_trim_start(string a)
{
    if (a == NULL)
        return false;

    if (a->value[0] != ' ')
        return true;
    unsigned long f = 0;
    while (a->value[f] == ' ')
    {
        f++;
    }

    size_t len = a->length - f;
    char *n = malloc(len + 1);
    if (n == NULL)
        return false;
    memcpy(n, &(a->value[f]), len + 1);

    __free_ptr(a->v_ctr);
    a->v_ctr = add_to_be_freed(n);
    if (!a->v_ctr)
    {
        free(n);
        return false;
    }

    a->value = n;
    a->length = len;
    a->end = n + a->length;
    return true;
}
string str_trim_end(string a)
{
    if (a == NULL)
        return NULL;
    if (a->length <= 0)
        return new_string();
    else if (a->end[-1] != ' ')
        return clone_str(a);

    unsigned long len = a->length;
    len--;
    while (len >= 0 && a->value[len] == ' ')
    {
        if (len == 0)
            break;
        len--;
    }
    if (len == 0)
        return new_string();

    char *n = malloc(len + 2);
    if (n == NULL)
        return NULL;
    memcpy(n, a->value, len + 1);
    n[len + 1] = 0;
    string rv = new_string();
    if (rv == NULL)
        return NULL;
    __free_ptr(rv->v_ctr);
    rv->v_ctr = add_to_be_freed(n);
    if (!rv->v_ctr)
    {
        free(n);
        __free_ptr(rv->ctr);
        return NULL;
    }

    rv->value = n;
    rv->length = len + 1;
    rv->end = n + rv->length;
    return rv;
}
bool assign_str_trim_end(string a)
{
    if (a == NULL)
        return false;
    if (a->length <= 0)
        return true;
    else if (a->end[-1] != ' ')
        return true;

    unsigned long len = a->length;
    len--;
    while (len >= 0 && a->value[len] == ' ')
        len--;

    char *n = malloc(len + 2);
    if (n == NULL)
        return NULL;
    memcpy(n, a->value, len + 1);
    n[len + 1] = 0;
    __free_ptr(a->v_ctr);
    a->v_ctr = add_to_be_freed(n);
    if (!a->v_ctr)
    {
        free(n);
        return false;
    }

    a->value = n;
    a->length = len + 1;
    a->end = n + a->length;
    return true;
}
string str_trim(string a)
{
    a = clone_str(a);
    if (!assign_str_trim_start(a))
        return new_string();
    if (!assign_str_trim_end(a))
        return new_string();
    return a;
}
bool assign_str_trim(string a)
{
    if (a == NULL)
        return false;
    if (!assign_str_trim_start(a))
        return false;
    if (!assign_str_trim_end(a))
        return false;
    return true;
}
string *str_split(string a, char sep, unsigned long *length)
{
    if (a == NULL || length == NULL)
        return false;
    *length = 0;
    char *x = malloc(a->length + 1);
    if (x == NULL)
        return NULL;
    memcpy(x, a->value, a->length + 1);
    char *tmp = x;

    string *rv = malloc(sizeof(string));
    if (rv == NULL)
    {
        free(x);
        return NULL;
    }

    for (unsigned long i = 0; i <= a->length; i++)
    {
        if (a->value[i] == sep || a->value[i] == 0)
        {
            x[i] = 0;
            string s = arr_to_str(tmp);
            if (s == NULL)
            {
                free(x);
                if (!add_to_be_freed(rv))
                {
                    *length = 0;
                    free(rv);
                    return NULL;
                }
                return rv;
            }
            *length = *length + 1;
            string *temp = realloc(rv, sizeof(string) * (*length));
            if (temp == NULL)
            {
                free(x);
                if (!add_to_be_freed(rv))
                {
                    *length = 0;
                    free(rv);
                    return NULL;
                }
                (*length)--;
                return rv;
            }
            rv = temp;
            rv[(*length) - 1] = s;
            tmp += s->length + 1;
        }
    }
    free(x);
    if (!add_to_be_freed(rv))
    {
        *length = 0;
        free(rv);
        return NULL;
    }
    return rv;
}
string str_to_lower(string s)
{
    if (s == NULL)
        return false;
    string rv = clone_str(s);
    if (rv == NULL)
        return NULL;
    for (unsigned long i = 0; i < s->length; i++)
    {
        if (s->value[i] >= 65 && s->value[i] <= 90)
            rv->value[i] += 32;
    }
    return rv;
}
bool assign_str_to_lower(string s)
{
    if (s == NULL)
        return false;
    for (unsigned long i = 0; i < s->length; i++)
    {
        if (s->value[i] >= 'A' && s->value[i] <= 'Z')
            s->value[i] += 'a' - 'A';
    }
    return true;
}
string str_to_upper(string s)
{
    if (s == NULL)
        return false;
    string rv = clone_str(s);
    if (rv == NULL)
        return NULL;
    for (unsigned long i = 0; i < s->length; i++)
    {
        if (s->value[i] >= 97 && s->value[i] <= 122)
            rv->value[i] -= 32;
    }
    return rv;
}
bool assign_str_to_upper(string s)
{
    if (s == NULL)
        return false;
    for (unsigned long i = 0; i < s->length; i++)
    {
        if (s->value[i] >= 'a' && s->value[i] <= 'z')
            s->value[i] -= 'a' - 'A';
    }
    return true;
}
string substring(string s, unsigned long start, unsigned long length)
{
    if (s->length < start + length)
        return NULL;
    string rv = new_string();
    __free_ptr(rv->v_ctr);
    char *x = malloc(length + 1);
    if (x == NULL)
    {
        __free_ptr(rv->ctr);
        return NULL;
    }
    memcpy(x, &(s->value[start]), length);
    rv->v_ctr = add_to_be_freed(x);
    if (!rv->v_ctr)
    {
        free(x);
        __free_ptr(rv->ctr);
        return NULL;
    }
    rv->value = x;
    rv->end = rv->value + length;
    rv->length = length;
    return rv;
}
string str_remove(string s, unsigned long start, unsigned long length)
{
    if (s->length < start + length)
        return NULL;
    string rv = new_string();
    if (rv == NULL)
        return NULL;
    __free_ptr(rv->v_ctr);
    rv->length = s->length - length;
    rv->value = malloc(rv->length);
    if (rv->value == NULL)
    {
        __free_ptr(rv->ctr);
        return NULL;
    }
    rv->v_ctr = add_to_be_freed(rv->value);
    if (!rv->v_ctr)
    {
        free(rv->value);
        __free_ptr(rv->ctr);
        return NULL;
    }
    rv->end = rv->value + length;
    memcpy(rv->value, s->value, start);
    memcpy(&(rv->value[start]), &(s->value[start + length]), rv->length - start);
    rv->value[rv->length] = 0;
    return rv;
}
unsigned long index_of_char(string s, char x)
{
    for (unsigned long i = 0; i < s->length; i++)
    {
        if (s->value[i] == x)
            return i;
    }
    return s->length;
}
unsigned long last_index_of_char(string s, char x)
{
    for (unsigned long i = s->length - 1; 1; i--)
    {
        if (s->value[i] == x)
            return i;
        if (i == 0)
            break;
    }
    return s->length;
}
unsigned long index_of_str(string s, string x)
{
    if (s->length < x->length)
        return s->length;
    for (unsigned long i = 0, l = s->length; i < l; i++)
    {
        unsigned long y = 0, m = x->length;
        for (; y < m; y++)
        {
            if (s->value[i + y] != x->value[y])
                break;
        }
        if (y == m)
            return i;
    }
    return s->length;
}
unsigned long last_index_of_str(string s, string x)
{
    if (s->length < x->length)
        return s->length;
    for (unsigned long i = s->length - x->length + 1; 1; i--)
    {
        unsigned long y = 0, m = x->length;
        for (; y < m; y++)
        {
            if (s->value[i + y] != x->value[y])
                break;
        }
        if (y == m)
            return i;
        if (i == 0)
            break;
    }
    return s->length;
}
unsigned long index_of_arr(string s, char *x)
{
    unsigned long x_len = strlen(x);
    if (s->length < x_len)
        return s->length;
    for (unsigned long i = 0, l = s->length; i < l; i++)
    {
        unsigned long y = 0, m = x_len;
        for (; y < m; y++)
        {
            if (s->value[i + y] != x[y])
                break;
        }
        if (y == m)
            return i;
    }
    return s->length;
}
unsigned long last_index_of_arr(string s, char *x)
{
    unsigned long x_len = strlen(x);
    if (s->length < x_len)
        return s->length;
    for (unsigned long i = s->length - x_len + 1; 1; i--)
    {
        unsigned long y = 0, m = x_len;
        for (; y < m; y++)
        {
            if (s->value[i + y] != x[y])
                break;
        }
        if (y == m)
            return i;
        if (i == 0)
            break;
    }
    return s->length;
}
bool isPalindrome(string s)
{
    for (unsigned long i = 0, l = s->length / 2; i < l; i++)
    {
        if (s->value[i] != s->value[s->length - 1 - i])
            return false;
    }
    return true;
}
bool str_reverse(string s)
{
    for (unsigned long x = 0, l = s->length / 2; x < l; x++)
    {
        char y = s->value[x];
        s->value[x] = s->value[s->length - 1 - x];
        s->value[s->length - 1 - x] = y;
    }
    return true;
}
char *str_value(string s)
{
    if (s == NULL)
        return NULL;
    return s->value;
}
unsigned long str_len(string s)
{
    if (s == NULL)
        return 0;
    return s->length;
}
void destroy_string(string s)
{
    if (s == NULL)
        return;
    __free_ptr(s->v_ctr);
    __free_ptr(s->ctr);
}
bool str_equals(string a, string b)
{
    if (a->length != b->length)
        return false;
    return strcmp(a->value, b->value) == 0;
}
char **str_split_arr(string s, char sep, unsigned long *length)
{
    if (s == NULL || length == NULL)
        return false;
    *length = 0;
    char *x = malloc(s->length + 1);
    if (x == NULL)
        return NULL;
    memcpy(x, s->value, s->length + 1);
    char *tmp = x;

    char **rv = malloc(sizeof(char *));
    if (rv == NULL)
    {
        free(x);
        return NULL;
    }

    for (unsigned long i = 0; i <= s->length; i++)
    {
        if (s->value[i] == sep || s->value[i] == 0)
        {
            x[i] = 0;
            *length = *length + 1;
            char **temp = realloc(rv, sizeof(char *) * (*length));
            if (temp == NULL)
            {
                if (!add_to_be_freed(x))
                {
                    free(x);
                    *length = 0;
                    free(rv);
                    return NULL;
                }
                if (!add_to_be_freed(rv))
                {
                    *length = 0;
                    free(rv);
                    return NULL;
                }
                (*length)--;
                return rv;
            }
            rv = temp;
            rv[(*length) - 1] = tmp;
            tmp += strlen(tmp) + 1;
        }
    }
    if (!add_to_be_freed(x))
    {
        free(x);
        *length = 0;
        free(rv);
        return NULL;
    }
    if (!add_to_be_freed(rv))
    {
        *length = 0;
        free(rv);
        return NULL;
    }
    return rv;
}
char **arr_split_arr(char *s, char sep, unsigned long *length)
{
    if (s == NULL || length == NULL)
        return false;
    *length = 0;
    unsigned long s_len = strlen(s);
    char *x = malloc(s_len + 1);
    if (x == NULL)
        return NULL;
    memcpy(x, s, s_len + 1);
    char *tmp = x;

    char **rv = malloc(sizeof(char *));
    if (rv == NULL)
    {
        free(x);
        return NULL;
    }

    for (unsigned long i = 0; i <= s_len; i++)
    {
        if (s[i] == sep || s[i] == 0)
        {
            x[i] = 0;
            *length = *length + 1;
            char **temp = realloc(rv, sizeof(char *) * (*length));
            if (temp == NULL)
            {
                if (!add_to_be_freed(x))
                {
                    free(x);
                    *length = 0;
                    free(rv);
                    return NULL;
                }
                if (!add_to_be_freed(rv))
                {
                    *length = 0;
                    free(rv);
                    return NULL;
                }
                (*length)--;
                return rv;
            }
            rv = temp;
            rv[(*length) - 1] = tmp;
            tmp += strlen(tmp) + 1;
        }
    }
    if (!add_to_be_freed(x))
    {
        free(x);
        *length = 0;
        free(rv);
        return NULL;
    }
    if (!add_to_be_freed(rv))
    {
        *length = 0;
        free(rv);
        return NULL;
    }
    return rv;
}