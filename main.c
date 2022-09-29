#include <limits.h>
#include <dirent.h>
#include "Strings/String.h"
#include "Exception/Exception.h"
#include "CLI/CLI.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdio.h>

#define BOLDWHITE S_ Bold White _E
#define BOLDRED S_ Bold Red _E
#define BOLDGREEN S_ Bold Green _E
#define RESET S_ Reset _E

#define VERSION 1.5

typedef struct
{
    char type; // 1 -> file 2 -> argument 3 -> compiler 4 -> set
    string value;
} property;

char **arguments_provided;
int arguments_provided_count = 0;

void default_usage();

void bar(int len);

void get_files(string s, char *set, FILE *f);

// returns 1 -> add, 2 -> remove, 3 -> replace
// sets rv to property type
char get_operation(char *rv, string *config);

string get_config(string *file_name);

void make(FILE *f);

void print_configurations();

u_long to_u_long(char *s);

bool get_type(char *rv);

int main(int argc, char *argv[])
{
    arguments_provided_count = argc;
    arguments_provided = &(argv[0]);

    try // error code 3 -> print nothing, 2 -> specific message, {2, 3}` -> default error message
    {
        if (argc == 1)
        {
            string config;
            string c_name = get_config(&config);
            FILE *f = fopen(str_value(config), "r");
            destroy_string(config);
            if (f == NULL)
            {
                throw(4, "");
            }
            printf(BOLDWHITE "Maker (v%.1f)\nBuilding config: " BOLDGREEN "%s\n" RESET, VERSION, str_value(c_name));
            destroy_string(c_name);
            make(f);
        }
        else if (argc == 2)
        {
            char type_required = 0;
            if (strcmp(arguments_provided[1], "-h") == 0 || strcmp(arguments_provided[1], "--help") == 0)
            {
                printf(BOLDWHITE "Maker (v%.1f) (Compiled on %s %s)\n\n" RESET, VERSION, __DATE__, __TIME__);
                printf(BOLDWHITE "Its required to set a configuration before adding any files, arguments or sets.\n" RESET);
                printf("%s %-30s" BOLDWHITE " : Sets the configuration or creates one.\n" RESET, arguments_provided[0], "set_config [config name]");
                printf("%s %-30s" BOLDWHITE " : Creates the configuration and sets the command. Treats first word as compiler, others as arguments.\n" RESET, arguments_provided[0], "set_config [config name] [cmd]");
                printf("%s %-30s" BOLDWHITE " : Prints the current configuration.\n\n" RESET, arguments_provided[0], "print_config");
                printf("%s %-30s" BOLDWHITE " : Prints all the configurations created.\n\n" RESET, arguments_provided[0], "print_configurations");

                printf(BOLDWHITE "%s %-30s : Compiles the current configuration.\n" RESET, arguments_provided[0], "");
                printf(BOLDWHITE "%s %-30s : Compiles the configs.\n\n" RESET, arguments_provided[0], "[config] [config] ...");

                printf(BOLDWHITE "Its required to set a compiler / a program to run.\n" RESET);
                printf("%s %-30s" BOLDWHITE " : Sets the compiler (or any program to run) of the current configuration.\n" RESET, arguments_provided[0], "set_compiler [compiler]");
                printf("%s %-30s" BOLDWHITE " : Prints the compiler set to the current configuration.\n\n" RESET, arguments_provided[0], "print_compiler");

                printf("%s %-30s" BOLDWHITE " : Adds the file to current configuration.\n" RESET, arguments_provided[0], "add_file [file]");
                printf("%s %-30s" BOLDWHITE " : Adds an argument to current configuration.\n" RESET, arguments_provided[0], "add_argument [argument]");
                printf("%s %-30s" BOLDWHITE " : Adds the set to current configuration.\n\n" RESET, arguments_provided[0], "add_set [set]");

                printf("%s %-30s" BOLDWHITE " : Prints all the files added to the current configuration.\n" RESET, arguments_provided[0], "print_files");
                printf("%s %-30s" BOLDWHITE " : Prints all the arguments added to the current configuration.\n" RESET, arguments_provided[0], "print_arguments");
                printf("%s %-30s" BOLDWHITE " : Prints all the sets added to the current configuration.\n\n" RESET, arguments_provided[0], "print_sets_added");

                printf("%s %-30s" BOLDWHITE " : Removes the file with the given id from current config.\n" RESET, arguments_provided[0], "remove_file [id]");
                printf("%s %-30s" BOLDWHITE " : Removes the argument with the given id from current config.\n" RESET, arguments_provided[0], "remove_argument [id]");
                printf("%s %-30s" BOLDWHITE " : Removes the set with the given id from current config.\n\n" RESET, arguments_provided[0], "remove_set_added [id]");

                printf("%s %-30s" BOLDWHITE " : Prints all the sets created.\n" RESET, arguments_provided[0], "print_sets");
                printf("%s %-30s" BOLDWHITE " : Adds the file to the existing set or creates one.\n" RESET, arguments_provided[0], "add_file [set] [file]");
                printf("%s %-30s" BOLDWHITE " : Prints the files added to a set.\n" RESET, arguments_provided[0], "print_files [set]");
                printf("%s %-30s" BOLDWHITE " : Removes the file with given index from the given set.\n" RESET, arguments_provided[0], "remove_file [set] [file id]");
                printf("%s %-30s" BOLDWHITE " : Deletes the set.\n\n" RESET, arguments_provided[0], "delete_set [set]");

                printf("\n");
            }
            else if (strcmp(arguments_provided[1], "print_config") == 0)
            {
                string temp;
                string config = get_config(&temp);
                destroy_string(temp);
                printf(BOLDWHITE "Last set configuration: %s\n" RESET, str_value(config));
            }
            else if (get_type(&type_required))
            {
                string f_name;
                destroy_string(get_config(&f_name));
                FILE *f = fopen(str_value(f_name), "r");
                if (f == NULL)
                    throw(4, "");
                u_long id = 1;
                char set = 0;
                while (true)
                {
                    char type;
                    if (!fread(&type, 1, 1, f))
                        break;
                    u_long len = 0;
                    fread(&len, sizeof(u_long), 1, f);
                    char *temp = malloc(len + 1);
                    if (temp == NULL)
                    {
                        fclose(f);
                        throw(2, "You ran out of memory");
                    }
                    fread(temp, 1, len, f);
                    temp[len] = 0;
                    if (type == type_required)
                    {
                        if (type_required == 3)
                        {
                            printf(BOLDWHITE "Compiler: %s\n" RESET, temp);
                            free(temp);
                            set = true;
                            break;
                        }
                        if (!set)
                        {
                            printf(BOLDWHITE "%7s | %s\n" RESET, "ID", type_required == 1 ? "File Name" : (type_required == 2 ? "Arguments" : (type_required == 4 ? "Sets" : "")));
                            set = 1;
                        }
                        printf("%7lu | %s\n", id, temp);
                        id++;
                    }
                    free(temp);
                }
                if (!set)
                {
                    printf(BOLDWHITE "No %s added.\n" RESET, type_required == 1 ? "Files" : (type_required == 2 ? "Arguments" : (type_required == 4 ? "Sets" : "Compiler")));
                }
                fclose(f);
            }
            else if (strcmp(argv[1], "print_sets") == 0)
            {
                struct passwd *w = getpwuid(getuid());
                if (w == NULL)
                    throw(4, "");
                char *home_dir = w->pw_dir;
                string f_name = concat_arr_arr(home_dir, "/.make_sets_config");
                FILE *f = fopen(str_value(f_name), "r");
                if (f == NULL)
                {
                    printf(BOLDRED "No sets created\n" RESET);
                    throw(3, "");
                }
                unsigned long id = 1;
                while (true)
                {
                    unsigned int n_files = 0;
                    if (!fread(&n_files, sizeof(unsigned int), 1, f))
                        break;
                    unsigned long len = 0;
                    fread(&len, sizeof(unsigned long), 1, f);
                    char *temp = malloc(len + 1);
                    fread(temp, 1, len, f);
                    temp[len] = 0;
                    if (id == 1)
                        printf(BOLDWHITE "%7s | Set name\n" RESET, "Id");
                    printf("%7lu | %s\n", id, temp);
                    for (unsigned int i = 0; i < n_files; i++)
                    {
                        free(temp);
                        len = 0;
                        fread(&len, sizeof(unsigned long), 1, f);
                        temp = malloc(len);
                        fread(temp, 1, len, f);
                    }
                    free(temp);
                    id++;
                }
                if (id == 1)
                {
                    printf(BOLDWHITE "No sets found\n" RESET);
                }
                fclose(f);
            }
            else if (strcmp(argv[1], "print_configurations") == 0)
            {
                print_configurations();
            }
            else
                default_usage();
        }
        else if (argc == 3)
        {
            if (strcmp(argv[1], "set_config") == 0)
            {
                {
                    string temp = arr_to_str(argv[2]);
                    if (temp == NULL)
                        throw(1, "");
                    assign_str_trim(temp);
                    if (str_len(temp) <= 0)
                        throw(2, "Invalid config name");
                    argv[2] = str_value(temp);
                }
                string file_name = concat_arr_arr("./.make_config/.config_", argv[2]);
                if (file_name == NULL)
                {
                    throw(1, "");
                }
                if (str_len(file_name) <= 23)
                {
                    throw(2, "Invalid configuration name.");
                }

                FILE *f = fopen("./.make_config/.make_config", "w");
                if (f == NULL)
                {
                    if (mkdir(".make_config", 0777) == -1)
                    {
                        throw(2, "Error creating make_config directory. Make sure there is no directory or file with the name \".make_config\".");
                    }
                    f = fopen("./.make_config/.make_config", "w");
                    if (f == NULL)
                        throw(1, "");
                }

                FILE *config = fopen(str_value(file_name), "a");
                if (config == NULL)
                {
                    fclose(f);
                    throw(1, "");
                }
                fclose(config);
                u_long length = strlen(argv[2]);
                fwrite(&length, sizeof(u_long), 1, f);
                fwrite(argv[2], 1, length, f);
                printf(BOLDWHITE "Successfully set the configuration to: %s\n" RESET, argv[2]);
                fclose(f);
            }
            else if (strcmp(argv[1], "delete_set") == 0)
            {
                {
                    string temp_name = arr_to_str(arguments_provided[2]);
                    if (temp_name == NULL)
                        throw(1, "");
                    assert_e(assign_str_trim(temp_name));
                    if (str_len(temp_name) <= 0)
                        throw(2, "Invalid set name");
                    arguments_provided[2] = str_value(temp_name);
                }
                struct passwd *w = getpwuid(getuid());
                if (w == NULL)
                    throw(4, "");
                char *home_dir = w->pw_dir;
                string f_name = concat_arr_arr(home_dir, "/.make_sets_config");
                FILE *f = fopen(str_value(f_name), "r");
                if (f == NULL)
                {
                    printf(BOLDRED "No sets created\n" RESET);
                    throw(3, "");
                }
                string temp_file_name = concat_arr_arr(home_dir, "/.temp_set_make");
                FILE *temp = fopen(str_value(temp_file_name), "w");
                if (temp == NULL)
                {
                    fclose(f);
                    throw(4, "");
                }
                bool removed = false;
                while (true)
                {
                    unsigned int i = 0;
                    if (!fread(&i, sizeof(unsigned int), 1, f))
                        break;
                    unsigned long length = 0;
                    fread(&length, sizeof(unsigned long), 1, f);
                    char *temp_name = malloc(length + 1);
                    if (temp_name == NULL)
                    {
                        fclose(f);
                        fclose(temp);
                        throw(4, "");
                    }
                    fread(temp_name, 1, length, f);
                    temp_name[length] = 0;
                    bool remove = false;
                    if (strcmp(temp_name, arguments_provided[2]) == 0)
                    {
                        remove = true;
                        removed = true;
                    }
                    if (!remove)
                    {
                        fwrite(&i, sizeof(unsigned int), 1, temp);
                        fwrite(&length, sizeof(unsigned long), 1, temp);
                        fwrite(temp_name, 1, length, temp);
                    }
                    else
                    {
                        printf(BOLDWHITE "Removed: %s\n" RESET, argv[2]);
                    }
                    for (unsigned int j = 0; j < i; j++)
                    {
                        fread(&length, sizeof(unsigned long), 1, f);
                        free(temp_name);
                        temp_name = malloc(length);
                        if (temp_name == NULL)
                        {
                            fclose(f);
                            fclose(temp);
                            throw(4, "");
                        }
                        fread(temp_name, 1, length, f);
                        if (!remove)
                        {
                            fwrite(&length, sizeof(unsigned long), 1, temp);
                            fwrite(temp_name, 1, length, temp);
                        }
                    }
                    free(temp_name);
                }
                if (!removed)
                {
                    fprintf(stderr, BOLDRED "Set not found\n" RESET);
                }
                fclose(temp);
                fclose(f);
                remove(str_value(f_name));
                rename(str_value(temp_file_name), str_value(f_name));
            }
            else if (strcmp(argv[1], "print_files") == 0)
            {
                struct passwd *w = getpwuid(getuid());
                if (w == NULL)
                    throw(4, "");
                char *home_dir = w->pw_dir;
                string f_name = concat_arr_arr(home_dir, "/.make_sets_config");
                FILE *f = fopen(str_value(f_name), "r");
                if (f == NULL)
                {
                    fprintf(stderr, BOLDRED "No sets created\n" RESET);
                    throw(3, "");
                }
                bool found = false;
                while (true)
                {
                    unsigned int f_count = 0;
                    if (!fread(&f_count, sizeof(unsigned int), 1, f))
                        break;
                    unsigned long len = 0;
                    fread(&len, sizeof(unsigned long), 1, f);
                    char *temp = malloc(len + 1);
                    if (temp == NULL)
                    {
                        fclose(f);
                        throw(2, "You ran out of memory.");
                    }
                    fread(temp, 1, len, f);
                    temp[len] = 0;

                    bool required = false;
                    if (strcmp(temp, argv[2]) == 0)
                    {
                        required = true;
                        found = true;
                    }
                    for (unsigned int i = 0; i < f_count; i++)
                    {
                        fread(&len, sizeof(unsigned long), 1, f);
                        free(temp);
                        temp = malloc(len + 1);
                        if (temp == NULL)
                        {
                            fclose(f);
                            throw(2, "You ran out of memory");
                        };
                        fread(temp, 1, len, f);
                        temp[len] = 0;
                        if (required)
                        {
                            if (i == 0)
                                printf(BOLDWHITE "%7s | File\n" RESET, "Id");
                            printf("%7u | %s\n", i + 1, temp);
                        }
                    }
                    free(temp);
                }
                if (!found)
                {
                    fprintf(stderr, BOLDRED "Set not found\n" RESET);
                }
                fclose(f);
            }
            else
            {
                char type = -1;
                string file_name;
                char operation = get_operation(&type, &file_name);
                u_long length = -1, id = -1;
                {
                    string temp_string = arr_to_str(arguments_provided[2]);
                    assert_e(assign_str_trim(temp_string));
                    length = str_len(temp_string);
                    arguments_provided[2] = str_value(temp_string);
                }
                if (length <= 0)
                    throw(2, type == 1 ? "Invalid File" : (type == 2 ? "Invalid Argument" : (type == 4 ? "Invalid Set" : "Invalid compiler")));

                FILE *f = fopen(str_value(file_name), operation == 1 ? "a" : "r");
                if (f == NULL)
                    throw(1, "");

                if (operation == 1)
                {
                    fwrite(&type, 1, 1, f);
                    fwrite(&length, sizeof(u_long), 1, f);
                    fwrite(arguments_provided[2], 1, length, f);
                    printf(BOLDWHITE "Added %s: %s\n" RESET, type == 1 ? "File" : (type == 2 ? "Argument" : (type == 4 ? "Set" : "")), arguments_provided[2]);
                }
                else
                {
                    FILE *temp = fopen("./.make_config/.temp", "w");
                    if (temp == NULL)
                    {
                        fclose(f);
                        throw(3, "");
                    }
                    if (operation == 3)
                        length = strlen(arguments_provided[2]);
                    else if (operation == 2)
                        id = to_u_long(arguments_provided[2]);
                    u_long c_id = 1;
                    bool replaced = false, removed = false;
                    while (true)
                    {
                        char c_type = 0;
                        if (!fread(&c_type, 1, 1, f))
                            break;
                        u_long len = 0;
                        fread(&len, sizeof(u_long), 1, f);
                        char *temp_name = malloc(len + 1);
                        if (temp_name == NULL)
                        {
                            fclose(temp);
                            remove("./.make_config/.temp");
                            throw(2, "You ran out of memory.");
                        }
                        fread(temp_name, 1, len, f);
                        temp_name[len] = 0;
                        if (type == c_type)
                        {
                            if (operation == 2)
                            {
                                if (id == c_id)
                                {
                                    printf(BOLDWHITE "Removed %s: %s\n" RESET, type == 1 ? "File" : (type == 2 ? "Argument" : (type == 4 ? "Set" : "")), temp_name);
                                    free(temp_name);
                                    c_id++;
                                    removed = true;
                                    continue;
                                }
                            }
                            else if (operation == 3)
                            {
                                printf(BOLDWHITE "Replaced %s with %s\n" RESET, temp_name, arguments_provided[2]);
                                free(temp_name);
                                replaced = true;
                                fwrite(&c_type, 1, 1, temp);
                                len = length;
                                fwrite(&len, sizeof(u_long), 1, temp);
                                temp_name = arguments_provided[2];
                                fwrite(temp_name, 1, len, temp);
                                continue;
                            }
                            c_id++;
                        }
                        fwrite(&c_type, 1, 1, temp);
                        fwrite(&len, sizeof(u_long), 1, temp);
                        fwrite(temp_name, 1, len, temp);
                        free(temp_name);
                    }
                    if (!replaced && operation == 3)
                    {
                        length = strlen(arguments_provided[2]);
                        fwrite(&type, 1, 1, temp);
                        fwrite(&length, sizeof(u_long), 1, temp);
                        fwrite(arguments_provided[2], 1, length, temp);
                        printf(BOLDWHITE "Added %s: %s\n" RESET, type == 1 ? "File" : (type == 2 ? "Argument" : "Compiler"), arguments_provided[2]);
                    }
                    if (!removed && operation == 2)
                    {
                        fprintf(stderr, BOLDRED "No %s with the given id was found\n" RESET, type == 1 ? "File" : (type == 2 ? "Argument" : (type == 4 ? "Set" : "Compiler")));
                    }
                    fclose(temp);
                    fclose(f);
                    remove(str_value(file_name));
                    rename("./.make_config/.temp", str_value(file_name));
                    f = NULL;
                }
                if (f != NULL)
                    fclose(f);
            }
        }
        else if (argc == 4)
        {
            if (strcmp(argv[1], "add_file") == 0)
            {
                string set_name = arr_to_str(argv[2]);
                assert_e(assign_str_trim(set_name));
                if (str_len(set_name) <= 0)
                    throw(2, "Invalid set name");
                arguments_provided[2] = str_value(set_name);

                string to_add = arr_to_str(arguments_provided[3]);
                assert_e(assign_str_trim(to_add));
                if (str_len(to_add) <= 0)
                    throw(2, "Invalid file name");

                struct passwd *w = getpwuid(getuid());
                if (w == NULL)
                    throw(4, "");
                char *home_dir = w->pw_dir;

                string home = concat_arr_arr(home_dir, "/.make_sets_config");

                char path[PATH_MAX + 1];
                if (getcwd(&(path[0]), sizeof(path)) == NULL)
                    throw(2, "Something went wrong.");
                u_long temp = strlen(&(path[0]));
                path[temp] = '/';
                path[temp + 1] = 0;

                assert_e(assign_insert_arr(to_add, 0, &(path[0])));

                FILE *existing = fopen(str_value(home), "r");
                if (existing == NULL)
                {
                    existing = fopen(str_value(home), "w");
                    if (existing == NULL)
                        throw(4, "");
                    unsigned int f_count = 1;
                    fwrite(&f_count, sizeof(unsigned int), 1, existing);
                    temp = str_len(set_name);
                    fwrite(&temp, sizeof(unsigned long), 1, existing);
                    fwrite(arguments_provided[2], 1, temp, existing);
                    temp = str_len(to_add);
                    fwrite(&temp, sizeof(unsigned long), 1, existing);
                    fwrite(str_value(to_add), 1, temp, existing);
                    printf(BOLDWHITE "Created %s and added %s\n" RESET, argv[2], argv[3]);
                    fclose(existing);
                    throw(3, "");
                }
                string temp_file_name = concat_arr_arr(home_dir, "/.temp_make_config");
                FILE *temp_file = fopen(str_value(temp_file_name), "w");
                if (temp_file == NULL)
                {
                    fclose(existing);
                    throw(3, "");
                }
                bool done = false;
                while (true)
                {
                    bool increase = false;
                    unsigned int n_files = 0;
                    if (!fread(&n_files, sizeof(unsigned int), 1, existing))
                    {
                        break;
                    }
                    unsigned long length = 0;
                    fread(&length, sizeof(unsigned long), 1, existing);
                    char *temp_lib = malloc(length + 1);
                    if (temp_lib == NULL)
                    {
                        fclose(existing);
                        fclose(temp_file);
                        throw(2, "You ran out of memory.");
                    }
                    temp_lib[length] = 0;
                    fread(temp_lib, 1, length, existing);

                    if (strcmp(temp_lib, arguments_provided[2]) == 0)
                    {
                        n_files++;
                        increase = true;
                    }
                    fwrite(&n_files, sizeof(unsigned int), 1, temp_file);
                    fwrite(&length, sizeof(unsigned long), 1, temp_file);
                    fwrite(temp_lib, 1, length, temp_file);
                    for (unsigned int i = 0, l = n_files - (increase ? 1 : 0); i < l; i++)
                    {
                        fread(&length, sizeof(unsigned long), 1, existing);
                        fwrite(&length, sizeof(unsigned long), 1, temp_file);
                        temp_lib = realloc(temp_lib, length);
                        fread(temp_lib, 1, length, existing);
                        fwrite(temp_lib, 1, length, temp_file);
                    }
                    if (increase)
                    {
                        length = str_len(to_add);
                        fwrite(&length, sizeof(unsigned long), 1, temp_file);
                        done = true;
                        fwrite(str_value(to_add), 1, length, temp_file);
                        printf(BOLDWHITE "Added %s to %s\n" RESET, argv[3], argv[2]);
                    }
                    free(temp_lib);
                }
                if (!done)
                {
                    unsigned int temp = 1;
                    fwrite(&temp, sizeof(unsigned int), 1, temp_file);
                    unsigned long lib_len = strlen(argv[2]);
                    fwrite(&lib_len, sizeof(unsigned long), 1, temp_file);
                    fwrite(argv[2], 1, lib_len, temp_file);
                    lib_len = str_len(to_add);
                    fwrite(&lib_len, sizeof(unsigned long), 1, temp_file);
                    fwrite(str_value(to_add), 1, lib_len, temp_file);
                    printf(BOLDWHITE "Created %s and added %s\n" RESET, argv[2], argv[3]);
                }
                fclose(temp_file);
                fclose(existing);
                remove(str_value(home));
                rename(str_value(temp_file_name), str_value(home));
            }
            else if (strcmp(argv[1], "remove_file") == 0)
            {
                unsigned int id = to_u_long(argv[3]);

                struct passwd *w = getpwuid(getuid());
                if (w == NULL)
                    throw(4, "");
                char *home_dir = w->pw_dir;
                string f_name = concat_arr_arr(home_dir, "/.make_sets_config");
                FILE *f = fopen(str_value(f_name), "r");
                if (f == NULL)
                {
                    fprintf(stderr, BOLDRED "No sets created\n" RESET);
                    throw(3, "");
                }

                string temp_file_name = concat_arr_arr(home_dir, "/.temp_set_make");
                FILE *temp = fopen(str_value(temp_file_name), "w");
                if (temp == NULL)
                {
                    fclose(f);
                    throw(4, "");
                }
                bool removed = false;
                while (true)
                {
                    unsigned int i = 0;
                    if (!fread(&i, sizeof(unsigned int), 1, f))
                        break;
                    unsigned long length = 0;
                    fread(&length, sizeof(unsigned long), 1, f);
                    char *temp_name = malloc(length + 1);
                    if (temp_name == NULL)
                    {
                        fclose(f);
                        fclose(temp);
                        throw(4, "");
                    }
                    fread(temp_name, 1, length, f);
                    temp_name[length] = 0;
                    bool remove = false;
                    if (strcmp(temp_name, arguments_provided[2]) == 0)
                    {
                        if (id > i || id == 0)
                        {
                        }
                        else
                        {
                            id--;
                            i--;
                            remove = true;
                        }
                    }
                    if (i == 0)
                    {
                        free(temp_name);
                        fread(&length, sizeof(unsigned long), 1, f);
                        temp_name = malloc(length + 1);
                        if (temp_name == NULL)
                        {
                            fclose(f);
                            fclose(temp);
                            throw(4, "");
                        }
                        fread(temp_name, 1, length, f);
                        temp_name[length] = 0;
                        printf(BOLDWHITE "Removed file: %s From %s\n" RESET, temp_name, argv[2]);
                        free(temp_name);
                        removed = true;
                        continue;
                    }
                    fwrite(&i, sizeof(unsigned int), 1, temp);
                    fwrite(&length, sizeof(unsigned long), 1, temp);
                    fwrite(temp_name, 1, length, temp);
                    i += remove ? 1 : 0;
                    for (unsigned int j = 0; j < i; j++)
                    {
                        fread(&length, sizeof(unsigned long), 1, f);
                        free(temp_name);
                        temp_name = malloc(length + 1);
                        if (temp_name == NULL)
                        {
                            fclose(f);
                            fclose(temp);
                            throw(4, "");
                        }
                        fread(temp_name, 1, length, f);
                        temp_name[length] = 0;
                        if (j != id || !remove)
                        {
                            fwrite(&length, sizeof(unsigned long), 1, temp);
                            fwrite(temp_name, 1, length, temp);
                        }
                        else
                        {
                            printf(BOLDWHITE "Removed file: %s From %s\n", temp_name, argv[2]);
                            removed = true;
                        }
                    }
                    free(temp_name);
                }
                if (!removed)
                {
                    fprintf(stderr, BOLDRED "File not found\n" RESET);
                }
                fclose(temp);
                fclose(f);
                remove(str_value(f_name));
                rename(str_value(temp_file_name), str_value(f_name));
            }
            else if (strcmp(argv[1], "set_config") == 0)
            {
                string command = arr_to_str(arguments_provided[3]);
                if (command == NULL)
                    throw(1, "");
                {
                    string temp = arr_to_str(argv[2]);
                    if (temp == NULL)
                        throw(1, "");
                    assert_e(assign_str_trim(temp));
                    if (str_len(temp) <= 0)
                        throw(2, "Invalid config name.");
                    argv[2] = str_value(temp);
                    assert_e(assign_str_trim(command));
                    if (str_len(command) <= 0)
                        throw(2, "Invalid command.");
                }
                string file_name = concat_arr_arr("./.make_config/.config_", argv[2]);
                if (file_name == NULL)
                {
                    throw(1, "");
                }
                if (str_len(file_name) <= 23)
                {
                    throw(2, "Invalid configuration name.");
                }

                unsigned long count;
                string *words = str_split(command, ' ', &count);
                if (words == NULL || count <= 0)
                    throw(1, "");

                FILE *f = fopen("./.make_config/.make_config", "w");
                if (f == NULL)
                {
                    if (mkdir(".make_config", 0777) == -1)
                    {
                        throw(2, "Error creating make_config directory. Make sure there is no directory or file with the name \".make_config\".");
                    }
                    f = fopen("./.make_config/.make_config", "w");
                    if (f == NULL)
                        throw(1, "");
                }

                FILE *config = fopen(str_value(file_name), "w");
                if (config == NULL)
                {
                    fclose(f);
                    throw(1, "");
                }

                char temp = 3;
                unsigned long temp_len = str_len(words[0]);
                fwrite(&temp, 1, 1, config);
                fwrite(&temp_len, sizeof(unsigned long), 1, config);
                fwrite(str_value(words[0]), 1, temp_len, config);
                for (unsigned long i = 1; i < count; i++)
                {
                    temp = 2;
                    temp_len = str_len(words[i]);
                    fwrite(&temp, 1, 1, config);
                    fwrite(&temp_len, sizeof(unsigned long), 1, config);
                    fwrite(str_value(words[i]), 1, temp_len, config);
                }
                fclose(config);
                u_long length = strlen(argv[2]);
                fwrite(&length, sizeof(u_long), 1, f);
                fwrite(argv[2], 1, length, f);
                printf(BOLDWHITE "Successfully set the configuration to: %s\n" RESET, argv[2]);
                fclose(f);
            }
            else
                default_usage();
        }
        else
        {
            default_usage();
        }
    }
    catch (2)
        fprintf(stderr, BOLDWHITE "An error has occurred -> " BOLDRED "%s\n" RESET, exception_handler->message);
    catch (3)
    {
    }
    catchall
        fprintf(stderr, BOLDRED "An error has occurred. Try again later.\n" RESET);
    endtry
}
string get_config(string *file_name)
{
    FILE *f = fopen("./.make_config/.make_config", "r");
    if (f == NULL)
    {
        fprintf(stderr, BOLDWHITE "Kindly set a configuration by executing %s set_config [config name]. Aborting\n" RESET, arguments_provided[0]);
        throw(3, "");
    }
    u_long length = 0;
    fread(&length, sizeof(u_long), 1, f);

    char *x = malloc(length + 1);
    if (x == NULL)
    {
        fclose(f);
        throw(2, "You ran out of memory.");
    }
    fread(x, 1, length, f);
    fclose(f);
    x[length] = 0;
    string rv = arr_to_str(x);
    free(x);
    if (rv == NULL)
        throw(2, "You ran out of memory.");
    string temp = concat_arr_arr("./.make_config/.config_", str_value(rv));
    f = fopen(str_value(temp), "r");
    if (f == NULL)
    {
        fprintf(stderr, BOLDWHITE "Configuration file missing, Kindly reset the config file by executing %s set_config %s\n" RESET, arguments_provided[0], str_value(rv));
        throw(3, "");
    }
    *file_name = temp;
    fclose(f);
    return rv;
}
char get_operation(char *rv, string *config)
{
    if (strcmp(arguments_provided[1], "add_file") == 0)
    {
        *rv = 1;
        destroy_string(get_config(config));
        return 1;
    }
    else if (strcmp(arguments_provided[1], "add_set") == 0)
    {
        *rv = 4;
        destroy_string(get_config(config));
        return 1;
    }
    else if (strcmp(arguments_provided[1], "add_argument") == 0)
    {
        *rv = 2;
        destroy_string(get_config(config));
        return 1;
    }
    else if (strcmp(arguments_provided[1], "set_compiler") == 0)
    {
        *rv = 3;
        destroy_string(get_config(config));
        return 3;
    }
    else if (strcmp(arguments_provided[1], "remove_set_added") == 0)
    {
        *rv = 4;
        destroy_string(get_config(config));
        return 2;
    }
    else if (strcmp(arguments_provided[1], "remove_file") == 0)
    {
        *rv = 1;
        destroy_string(get_config(config));
        return 2;
    }
    else if (strcmp(arguments_provided[1], "remove_argument") == 0)
    {
        *rv = 2;
        destroy_string(get_config(config));
        return 2;
    }
    else
        default_usage();
    return 0;
}
u_long to_u_long(char *s)
{
    u_long rv = 0;
    for (unsigned long i = 0; s[i]; i++)
    {
        rv *= 10;
        if (s[i] >= '0' && s[i] <= '9')
            rv += s[i] - '0';
        else
            throw(2, "Invalid id");
    }
    return rv;
}
void get_files(string s, char *set, FILE *file)
{
    struct passwd *w = getpwuid(getuid());
    if (w == NULL)
        throw(4, "");
    char *home_dir = w->pw_dir;
    string f_name = concat_arr_arr(home_dir, "/.make_sets_config");
    FILE *f = fopen(str_value(f_name), "r");
    if (f == NULL)
    {
        fprintf(stderr, BOLDRED "No sets created. Set %s not found.\n" RESET, set);
        free(set);
        fclose(file);
        throw(3, "");
    }
    bool found = false;
    while (true)
    {
        unsigned int f_count = 0;
        if (!fread(&f_count, sizeof(unsigned int), 1, f))
            break;
        unsigned long len = 0;
        fread(&len, sizeof(unsigned long), 1, f);
        char *temp = malloc(len + 1);
        if (temp == NULL)
        {
            fclose(f);
            free(set);
            fclose(file);
            throw(2, "You ran out of memory.");
        }
        fread(temp, 1, len, f);
        temp[len] = 0;

        bool required = false;
        if (strcmp(temp, set) == 0)
        {
            required = true;
            found = true;
        }
        for (unsigned int i = 0; i < f_count; i++)
        {
            fread(&len, sizeof(unsigned long), 1, f);
            free(temp);
            temp = malloc(len + 1);
            if (temp == NULL)
            {
                fclose(f);
                free(set);
                fclose(file);
                throw(2, "You ran out of memory");
            };
            fread(temp, 1, len, f);
            temp[len] = 0;
            if (required)
            {
                string b = concat_arr_arr(temp, " ");
                assign_concat_str_str(s, b);
                destroy_string(b);
            }
        }
        free(temp);
    }
    if (!found)
    {
        fprintf(stderr, BOLDRED "Set %s not found\n" RESET, set);
        free(set);
        fclose(file);
        throw(3, "");
    }
    fclose(f);
}
void bar(int len)
{
    printf(BOLDWHITE "\n");
    while (len--)
    {
        printf("%c", '#');
    }
    printf("\n");
    printf(RESET);
}
void make(FILE *f)
{
    char *compiler = NULL;
    string command = arr_to_str(" ");
    while (true)
    {
        char type;
        if (!fread(&type, 1, 1, f))
            break;
        u_long len;
        fread(&len, sizeof(u_long), 1, f);
        char *temp = malloc(len + 2);
        fread(temp, 1, len, f);
        if (type == 1 || type == 2)
        {
            temp[len] = ' ';
            temp[len + 1] = 0;
            assign_concat_str_arr(command, temp);
        }
        else if (type == 4)
        {
            temp[len] = 0;
            get_files(command, temp, f);
        }
        else if (type == 3)
        {
            compiler = temp;
            compiler[len] = 0;
            temp = NULL;
        }
        free(temp);
    }
    fclose(f);
    if (compiler == NULL)
    {
        fprintf(stderr, BOLDRED "Kindly set a compiler by calling: %s set_compiler [compiler]. Aborting. Use -h for help.\n" RESET, arguments_provided[0]);
        throw(3, "");
    }
    else
    {
        assign_insert_arr(command, 0, compiler);
        free(compiler);
        printf(BOLDWHITE "Executing:" BOLDGREEN " %s" RESET "\n", str_value(command));
        system(str_value(command));
        printf(BOLDGREEN "DONE\n" RESET);
    }
}
void default_usage()
{
    printf(BOLDWHITE "Maker (v%.1f)\n" RESET, VERSION);
    for (int i = 1; i < arguments_provided_count; i++)
    {
        string config_name = arr_to_str(arguments_provided[i]);
        if (config_name == NULL || !assign_str_trim(config_name))
            throw(1, "");
        if (str_len(config_name) <= 0)
            throw(2, "Invalid config name");
        arguments_provided[i] = str_value(config_name);

        string config_file_name = concat_arr_arr(".make_config/.config_", arguments_provided[i]);
        if (config_file_name == NULL)
            throw(2, "Memory error");

        FILE *f = fopen(str_value(config_file_name), "r");
        destroy_string(config_file_name);
        if (f == NULL)
        {
            printf(BOLDRED "\nConfiguration: %s does not exist. Set the config by set_config. Aborting. Try %s -h for help.\n" RESET, arguments_provided[i], arguments_provided[0]);
            throw(3, "");
        }
        printf(BOLDWHITE "\nBuilding config: " BOLDGREEN "%s\n" RESET, arguments_provided[i]);
        make(f);
    }
    throw(3, "");
}
bool get_type(char *rv)
{
    if (strcmp(arguments_provided[1], "print_files") == 0)
    {
        *rv = 1;
        return true;
    }
    else if (strcmp(arguments_provided[1], "print_arguments") == 0)
    {
        *rv = 2;
        return true;
    }
    else if (strcmp(arguments_provided[1], "print_compiler") == 0)
    {
        *rv = 3;
        return true;
    }
    else if (strcmp(arguments_provided[1], "print_sets_added") == 0)
    {
        *rv = 4;
        return true;
    }
    return false;
}
void print_configurations()
{
    struct dirent *d;
    DIR *directory = opendir(".make_config");
    if (directory == NULL)
    {
        throw(2, "Could not open configurations directory.");
    }
    bool printed = false;
    int count = 0;
    while ((d = readdir(directory)) != NULL)
    {
        string temp = arr_to_str(d->d_name);
        if (starts_with_arr(temp, ".config_"))
        {
            if (!printed)
            {
                printf(BOLDWHITE "%5s | %-10s\n", "Id", "Name" RESET);
                printed = true;
            }
            string new = str_remove(temp, 0, 8);
            destroy_string(temp);
            printf("%5d | %-10s\n", ++count, str_value(new));
            destroy_string(new);
        }
        else
            destroy_string(temp);
    }
    if (!printed)
    {
        printf(BOLDRED "No Configurations created.\n" RESET);
    }
    closedir(directory);
}