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

	if (argc == 1)
	{
		errno = 0;
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

				cmds = parser(cmds);
				if (cmds)
				{
					/*Search the PATH for directory with the file before executing*/
					flush_io();
					executor(cmds);
					free_args(cmds);
				}
				else if (err == -1)
					write(STDERR_FILENO, "Could not parse command\n", 25);
			}
		}
		else
		{
			if (errno == ENOTTY)
			{
				cmds = parser(cmds);
				if (cmds)
				{
					/*Search the PATH for directory with the file before executing*/
					flush_io();
					executor(cmds);
					free_args(cmds);
				}
				else if (err == -1)
					write(STDERR_FILENO, "Could not parse command\n", 25);
			}
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
