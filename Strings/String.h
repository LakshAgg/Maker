/**
 * @file String.h
 * @author Laksh Aggarwal (aggarwallaksh54@gmail.com)
 * @brief Header file for strings library. Includes a few cool functions for string operations.
 * @version 0.1
 * @date 2022-03-29
 * 
 * 
 */
#ifndef STR_H
#define STR_H
#include <stdbool.h>

typedef struct S_Str _string;

/**
 * @brief Stores a string and other info
 */
typedef _string *string;

/**
 * @brief creates an empty string. returns null in case of errors.
 * @return string 
 */
string new_string();

/**
 * @brief Converts arr to string. returns NULL if unsuccessful.
 * @param arr 
 * @return string 
 */
string arr_to_str(char *arr);

/**
 * @brief assigns s the value v. returns bool success.
 * @param s 
 * @param v 
 * @return true 
 * @return false 
 */
bool assign_str(string s, char *v);

/**
 * @brief concatenates a abd b, returns new string
 * @param a 
 * @param b 
 * @return string 
 */
string concat_str_str(string a, string b);

/**
 * @brief adds b to a, returns true if successful.
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool assign_concat_str_str(string a, string b);

/**
 * @brief concatenates a abd b, returns new string
 * @param a 
 * @param b 
 * @return string 
 */
string concat_str_arr(string a, char *b);

/**
 * @brief adds b to a, returns true if successful.
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool assign_concat_str_arr(string a, char *b);

/**
 * @brief concatenates a and b. returns new string
 * @param a 
 * @param b 
 * @return string 
 */
string concat_arr_arr(char *a, char *b);

/**
 * @brief returns a clone of s.
 * @param v 
 * @return string 
 */
string clone_str(string v);

/**
 * @brief clones {from} to {to}. returns true if successful. Same as assign_str
 * @param to 
 * @param from 
 * @return true 
 * @return false 
 */
bool clone_to(string to, string from);

/**
 * @brief returns true, if b is substring of a
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool contains_str(string a, string b);

/**
 * @brief returns true, if b is substring of a
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool contains_arr(string a, char *b);

/**
 * @brief returns true if a ends with b
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool ends_with_str(string a, string b);

/**
 * @brief returns true if a ends with b
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool ends_with_arr(string a, char *b);

/**
 * @brief returns true if a starts with b.
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool starts_with_str(string a, string b);

/**
 * @brief returns true if a starts with b.
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool starts_with_arr(string a, char *b);

/**
 * @brief inserts b in a at specified position and returns pointer to new string
 * @param a 
 * @param index 
 * @param b 
 * @return string 
 */
string insert_str(string a, unsigned long index, string b);

/**
 * @brief inserts b into a at specified position.
 * @param a 
 * @param index 
 * @param b 
 * @return true 
 * @return false 
 */
bool assign_insert_str(string a, unsigned long index, string b);

/**
 * @brief returns copy of a with b inserted at specified position
 * @param a 
 * @param index 
 * @param b 
 * @return string 
 */
string insert_arr(string a, unsigned long index, char *b);

/**
 * @brief inserts b into a at specified position
 * @param a 
 * @param index 
 * @param b 
 * @return true 
 * @return false 
 */
bool assign_insert_arr(string a, unsigned long index, char *b);

/**
 * @brief returns a copy of a without beginning spaces
 * @param a 
 * @return string 
 */
string str_trim_start(string a);

/**
 * @brief removes beginning spaces from a
 * @param a 
 * @return true 
 * @return false 
 */
bool assign_str_trim_start(string a);

/**
 * @brief returns a copy of a without trailing spaces
 * @param a 
 * @return string 
 */
string str_trim_end(string a);

/**
 * @brief removes trailing spaces from a
 * @param a 
 * @return true 
 * @return false 
 */
bool assign_str_trim_end(string a);

/**
 * @brief returns a copy of a without trailing and beginning spaces.
 * @param a 
 * @return string 
 */
string str_trim(string a);

/**
 * @brief removes beginning and trailing spaces from a.
 * @param a 
 * @return true 
 * @return false 
 */
bool assign_str_trim(string a);

// splits a into an array of strings and stores its length
string *str_split(string a, char sep, unsigned long *length);

/**
 * @brief splits s and returns array of char *. stores length in length.
 * @param s 
 * @param sep 
 * @param length 
 * @return char** 
 */
char **str_split_arr(string s, char sep, unsigned long *length);

/**
 * @brief splits s and returns the array of char *. stores length in length.
 * @param s 
 * @param sep 
 * @param length 
 * @return char** 
 */
char **arr_split_arr(char *s, char sep, unsigned long *length);

/**
 * @brief returns uppercase copy of s.
 * @param s 
 * @return string 
 */
string str_to_upper(string s);

/**
 * @brief returns lowercase copy of s.
 * @param s 
 * @return string 
 */
string str_to_lower(string s);

/**
 * @brief cunverts s to lower case. returns true if successful
 * @param s 
 * @return true 
 * @return false 
 */
bool assign_str_to_lower(string s);

/**
 * @brief converts s to uppercase. returns true if successful.
 * @param s 
 * @return true 
 * @return false 
 */
bool assign_str_to_upper(string s);

/**
 * @brief returns substring of s
 * @param s 
 * @param start 
 * @param length 
 * @return string 
 */
string substring(string s, unsigned long start, unsigned long length);

/**
 * @brief returns s excluding the region specified
 * @param s 
 * @param start 
 * @param length 
 * @return string 
 */
string str_remove(string s, unsigned long start, unsigned long length);

/**
 * @brief returns index of first char x it finds. returns index of null character if not found.
 * @param s 
 * @param x 
 * @return unsigned long 
 */
unsigned long index_of_char(string s, char x);

/**
 * @brief returns index of last char x in s.
 * returns index of null character if not found
 * @param s 
 * @param x 
 * @return unsigned long 
 */
unsigned long last_index_of_char(string s, char x);

/**
 * @brief Returns index of first occurrence of x in s. returns index of NULL character if not found.
 * @param s 
 * @param x 
 * @return unsigned long 
 */
unsigned long index_of_str(string s, string x);

/**
 * @brief Returns index of first occurrence of x in s. returns index of NULL character if not found.
 * @param s 
 * @param x 
 * @return unsigned long 
 */
unsigned long index_of_arr(string s, char *x);

/**
 * @brief returns index of last occurrence of x in s. returns index of NULL character if not found.
 * @param s 
 * @param x 
 * @return unsigned long 
 */
unsigned long last_index_of_str(string s, string x);

/**
 * @brief returns index of last occurrence of x in s. returns index of NULL character if not found.
 * @param s 
 * @param x 
 * @return unsigned long 
 */
unsigned long last_index_of_arr(string s, char *x);

/**
 * @brief returns if s is a palindrome.
 * @param s 
 * @return true 
 * @return false 
 */
bool isPalindrome(string s);

/**
 * @brief Reverses the string and returns true if successful.
 * @param s 
 * @return true 
 * @return false 
 */
bool str_reverse(string s);

/**
 * @brief Returns the char *value of the string.
 * @param s 
 * @return char* 
 */
char *str_value(string s);

/**
 * @brief returns the length of the string, pre calculated.
 * @param s 
 * @return unsigned long 
 */
unsigned long str_len(string s);

/**
 * @brief Frees the memory allocated for s.
 * @param s 
 */
void destroy_string(string s);

/**
 * @brief returns true if a equals b
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool str_equals(string a, string b);

    #ifndef valueof
        #define valueof(s) str_value(s)
    #endif
#endif