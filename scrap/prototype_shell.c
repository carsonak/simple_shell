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
	int err, i = 0;
	cmd_str *c_s = NULL, *walk = c_s;

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

			err = parser(c_s, &cmds);
			if (err == 0)
			{
				for (i = 0; walk; i++)
				{
					if (walk->command)
						printf("%s\n", walk->command);

					walk = walk->next;
				}

				free_list(c_s);
				executor(cmds);
			}
			else
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
