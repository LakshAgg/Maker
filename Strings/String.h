#ifndef STR_H
#define STR_H
#include <stdbool.h>

// Stores a string and its other info
typedef struct S_Str string;

// creates an empty string and returns its pointer
// returns null in case of errors
string *new_string();

// converts array of characters [aka string] to string and returns its pointer
// returns null in case of errors
string *arr_to_str(char *arr);

// assigns a char array [akka string] to string s
// returns true if successful
bool assign_str(string *s, char *v);

// concats string a and b returns pointer to new string
// returns null in case of errors;
string *concat_str_str(string *a, string *b);

// concats string a and string b, sets a to the new string
// returns true if successful
bool assign_concat_str_str(string *a, string *b);

// concats string a and char * b returns pointer to new string
// returns null in case of errors;
string *concat_str_arr(string *a, char *b);

// concats string a and char array b, assigns a the new value;
// returns true if successful
bool assign_concat_str_arr(string *a, char *b);

// concats char *a and b returns pointer to new string
// returns null in case of errors;
string *concat_arr_arr(char *a, char *b);

// clones v and returns its pointer
// returns null in case of errors
string *clone_str(string *v);

// clones from and returns true if success
bool clone_to(string *to, string *from);

// check if a contains b as substring
bool contains_str(string *a, string *b);

// check if a contains b as substring
bool contains_arr(string *a, char *b);

// check if a ends with b
bool ends_with_str(string *a, string *b);

// check if a ends with b
bool ends_with_arr(string *a, char *b);

// check if a starts with b
bool starts_with_str(string *a, string *b);

// check if a starts with b
bool starts_with_arr(string *a, char *b);

// inserts b at index of a and returns pointer to new string 
// returns null in case of errors
string *insert_str(string *a, unsigned long index, string *b);

// inserts b at index of a and assigns a the new value
// returns true if successful
bool assign_insert_str(string *a, unsigned long index, string *b);

// inserts b at index of a and returns pointer to new string 
// returns null in case of errors
string *insert_arr(string *a, unsigned long index, char *b);

// inserts b at index of a and assigns a the new value
// returns true if successful
bool assign_insert_arr(string *a, unsigned long index, char *b);

// trims and returns pointer to new string
string *str_trim_start(string *a);

// trims and assigns a the new string
// returns true if successful
bool assign_str_trim_start(string *a);

// trims and returns pointer to new string
string *str_trim_end(string *a);

// trims and assigns a the new value
// returns true if successful
bool assign_str_trim_end(string *a);

// trims and returns pointer to new string
string *str_trim(string *a);

// trims and assigns a the new value;
// returns true if successful
bool assign_str_trim(string *a);

// splits a into an array of strings and stores its length
string **str_split(string *a, char sep, unsigned long *length);

// returns uppercase version of s
string *str_to_upper(string *s);

// returns uppercase version of s
string *str_to_lower(string *s);

// converts s to lower case and assigns it the new value
// returns true if successful
bool assign_str_to_lower(string *s);

// converts s to upper case and assigns it the new value
// returns true id successful
bool assign_str_to_upper(string *s);

// returns substring of s
string *substring(string *s, unsigned long start, unsigned long length);

// returns string excluding specified region
string *str_remove(string *s, unsigned long start, unsigned long length);

// returns index of first char x it finds
// returns index of null character if not found
unsigned long index_of_char(string *s, char x);

// returns index of last char x it finds
// returns index of null character if not found
unsigned long last_index_of_char(string *s, char x);

// returns index of first substring x it finds
// returns index of null character if not found
unsigned long index_of_str(string *s, string *x);

// returns index of last substring x it finds
// returns index of null character if not found
unsigned long last_index_of_str(string *s, string *x);

// returns index of last substring x it finds
// returns index of null character if not found
unsigned long index_of_arr(string *s, char *x);

// returns index of last substring x it finds
// returns index of null character if not found
unsigned long last_index_of_arr(string *s, char *x);

bool isPalindrome(string *s);

// reverses the string and returns true if successful
bool str_reverse(string *s);

// returns the string which you can use
char *str_value(string *s);

// returns string's length
unsigned long str_len(string *s);

void destroy_string(string *s);
#endif