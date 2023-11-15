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
	long int err;

	errno = 0;
	if (argc == 1)
	{
		err = isatty(STDIN_FILENO);
		if (err)
		{
			while (1)
			{
				err = write(1, "$!", 2);
				if (err == -1)
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
		/**
		 * Should be able to detrmine if command exist via searching
		 * the PATH environment variable before calling the executor.
		 */
		cmds = &argv[1];
		executor(cmds);
	}

	return (0);
}

/**
 * parse_n_exec - parses std input and executes the commands
 */
void parse_n_exec(void)
{
	char **cmds = NULL;

	cmds = parser(cmds);
	if (cmds)
	{
		/*Search the PATH for directory with the file before executing*/
		flush_io();
		executor(cmds);
		free_args(cmds);
	}
	else
		write(STDERR_FILENO, "Couldn't parse command\n", 24);
}
