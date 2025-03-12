#ifndef SIMPLE_SHELL_TYPES_H
#define SIMPLE_SHELL_TYPES_H

/**
 * struct shell_variable - a variable type.
 * @name: name of the variable.
 * @value: contents of the variable.
 */
struct shell_variable
{
	char *name;
	char *value;
};

typedef struct shell_variable shell_variable;

/**
 * struct shell_command - a shell command and parameters.
 * @cmd: path to binary or function to execute.
 * @argv: arguments to the command.
 */
struct shell_command
{
	char *cmd;
	char **argv;
};

typedef struct shell_command shell_command;

#endif /* SIMPLE_SHELL_TYPES_H */
