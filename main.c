#include "shell.h"

/**
 * main - a simple non-interactive shell
 * @argc: number of arguments passed to the program
 * @argv: the arguments passed
 *
 * Return: always 0
 */
int main(int argc, char *argv[])
{
	char **cmds = NULL;

	errno = 0;
	if (argc == 1)
	{
		if (isatty(STDIN_FILENO))
		{
			while (1)
			{
				if (write(1, "$!", 2) == -1)
				{
					perror("Whoops");
					return (EXIT_FAILURE);
				}
				parse_n_exec();
			}
		}
		else
		{
			if (errno == ENOTTY)
				parse_n_exec();
			else
				perror("Corrupted input");
		}
	}
	else
	{
		cmds = &argv[1];
		if (cmds[0][0] != '/')
			cmds[0] = isPath(cmds[0]);

		if (cmds[0])
		{
			executor(cmds);
			free(cmds[0]);
		}
		else if ((errno == 0) && !cmds[0])
			write(STDERR_FILENO, "Command not found\n", 19);
	}
	return (0);
}

/**
 * parse_n_exec - parses std input and executes the commands
 */
void parse_n_exec(void)
{
	char **cmds = NULL, *backup = NULL;

	cmds = parser(cmds);
	if (cmds)
	{
		if (cmds[0][0] != '/')
		{
			backup = cmds[0];
			cmds[0] = isPath(cmds[0]);
			free(backup);
		}

		flush_io();
		if (cmds[0])
			executor(cmds);
		else if ((errno == 0) && !cmds[0])
			write(STDERR_FILENO, "Command not found\n", 19);

		free_args(cmds);
	}
	else
		write(STDERR_FILENO, "Couldn't parse command\n", 24);
}
