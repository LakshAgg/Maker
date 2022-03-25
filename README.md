# Maker
This is a simple and useless program which lets you compile programs. You can create configurations for different commands. You can also group files by making sets which can be included in your programs.

## Installation
Execute:
```
gcc main.c Basic.c Exception/Exception.c Strings/String.c -o Maker
```
And add the file Maker to your environment paths.

## Usage
You can print all the commands with their use by `Maker -h`.

## Sets
Sets are just collection of files. You can create sets for any files and include them in any project by just adding the set.

Sets can be created by: `Maker add_file [set] [file_name]`. This will add the file if set exists or creates the set.

Sets can be printed by: `Maker print_sets`.

Files in a set can be printed by: `Maker print_files [set]`. This will print the files in the set with their index.

File can be removed from a set by: `Maker remove_file [set] [index]`. Index can be obtained by printing the files.

Complete set can be deleted by: `Maker remove_set [set]`.

## Config
For every project you can create configurations. Example: for testing. This will let you keep commands saparated for different purposes.

A config can be set or created by: `Maker set_config [config name]`. Last set config can be printed by `Maker print_config`;

For every config you need to specify compiler by: `Maker set_compiler [compiler]`. You can print the set compiler by `Maker print_compiler`.

`Maker` simply compiles the last set config.

In every config source files can be added by: `Maker add_file [file]`. This will add file to the last set config.

Files of a config can be printed by: `Maker print_files`. This will print all the files of last set config with their index.

File can be removed from a config by: `Maker remove_file [index]`. This will remove file from the last set config.

Similarly sets operations are: `add_set [set]` `print_sets_added` `remove_set_added [id]`.

Similarly argument operations are: `add_argument [argument]` `print_arguments` `remove_argument [id]`.
