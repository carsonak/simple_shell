#include "simple_shell.h"

/**
 * main - Entry point
 * @argc: number of arguments passed to the program.
 * @argv: pointer to the strings of command line arguments.
 *
 * Return: 0
 */
int main(int argc, char *argv[])
{
	/* Parse arguments to shell. */
	/* Parse commands to the shell line by line / statement by statement */
	/*	If ( unrecognised option ) read commands from script */
	/*	If ( -i | (argc == 1 && stdin istty) ) interactive mode */
	/*	else read commands from stdin */
	int i = 0;
	pid_t pid = fork();
	char *s = malloc(10);

	(void)argc;
	(void)argv;
	if (pid)
	{
		wait(NULL);
		for (i = 0; i < 15; i++)
			s[i] = 'A';
	}

	return (0);
}
