#include "shell.h"

/**
 * shell_exit - Exits the shell.
 */
void shell_exit(void)
{
	printf("Exiting the shell.\n");
	exit(0);
}

/**
 * remove_newline - Removes the trailing newline character from a string.
 * @str: The input string.
 */
void remove_newline(char *str)
{
	size_t length = strlen(str);

	if (length > 0 && str[length - 1] == '\n')
	{
		str[length - 1] = '\0';
	}
}

/**
 * main - Main function for the simple shell.
 * 
 * Return: Always 0.
 */
int main(void)
{
	char input[MAX_INPUT_SIZE];

	while (1)
	{
		printf("$ ");
		fgets(input, sizeof(input), stdin);

		remove_newline(input);

		if (strcmp(input, "exit") == 0)
		{
			shell_exit();
		}

		system(input);
	}

	return (0);
}

