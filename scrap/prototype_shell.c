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
	char **cmds;
	char buf[1024];
	int err;

	if (argc == 1)
	{
		while (1)
		{
			err = write(1, "$! ", 2);
			if (err == -1)
			{
				perror("Whoops");
				return (EXIT_FAILURE);
			}
			err = read(0, buf, 1024);
			if (err == -1)
			{
				perror("Failed to read");
				return (EXIT_FAILURE);
			}

			err = write(1, buf, strlen(buf));
			if (err == -1)
			{
				perror("Failed to write");
				return (EXIT_FAILURE);
			}

			memset(buf, '\0', 1024);
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
