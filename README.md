# SIMPLE_SHELL

This is a simple shell program that has the show features.
Compile command: gcc -std=c17 -g -pedantic -Wall -Werror -Wextra -fdiagnostics-color=always
-fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all ./*.c -o ./s_sh

## ABOUT

This program has been written in [Betty coding style](https://github.com/alx-tools/Betty/wiki#1-coding-style) and [documentation](https://github.com/alx-tools/Betty/wiki#2-documentation).

## v0.0

The program has an interactive and non-interactive mode.
In interactive mode:

- Displays a prompt and waits for the user to type a command. A command line
    always ends with a new line.
- The prompt is displayed again each time a command has been executed.
- The command lines are simple, no semicolons, no pipes, no redirections or any
    other advanced features.
- The command lines are made only of one word. Doesn't handle arguments to
    commands.
- If an executable cannot be found, prints an error message and displays the
    prompt again.
- Handles errors.
- Handles the “end of file” condition (Ctrl+D)

## v0.1

Can now handle command line arguments.

## v0.2

If commands given to the shell are not absolute pathnames. The shell searches
for the file in the PATH evironment variable. Fork will not be called when the
command is not found.

## v0.3

## v1.0

## v.0.1.1

The program uses a custom getline function [_getline](./_getline.c "_getline.c")

## v0.2.1

The program uses a custom strtok function [_strtok](./_strtok.c "_getline.c")
