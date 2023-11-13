#include "scrap.h"

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
		while (1)
		{
			err = write(1, "$!", 2);
			if (err == -1)
			{
				perror("Whoops");
				return (EXIT_FAILURE);
			}

			err = parser(&cmds);
			if (err > 0)
			{
				/*Search the PATH for directory with the file before executing*/
				executor(cmds);
				free(cmds);
			}
			else if (err == -1)
				write(STDERR_FILENO, "Could not parse command\n", 25);
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
