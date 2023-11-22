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
	int err = 0;

	errno = 0;
	E_status(EXIT_SUCCESS);
	if (argc == 1)
	{
		while (1)
		{
			errno = 0;
			if (is_EOF(-1))
				break;

			if (isatty(STDIN_FILENO))
				prompt();

			if (!parse_n_exec())
				break;
		}
	}
	else
	{
		cmds = &argv[1];
		err = isPath(&cmds[0]);
		if (err == 1)
			executor(cmds);
		else if (!err)
		{
			write(STDERR_FILENO, "Command not found\n", 19);
			E_status(127);
		}
		else if (err == -1)
			perror("isPath() failure");
	}
	return (E_status(1000));
}

/**
 * parse_n_exec - parses std input and executes the commands
 *
 * Return: number of bytes processed, -1 on failure.
 */
ssize_t parse_n_exec(void)
{
	char **cmds = NULL, *line = NULL;
	ssize_t err = 0, ln_sz = 0, byts = 0;

	errno = 0;
	byts = _getline(&line, &ln_sz, STDIN_FILENO);
	if (byts == -1)
		perror("_getline() error");
	else if (line)
	{
		if (ln_sz == 1 && line[0] == '\n')
		{
			free(line);
			return (byts);
		}

		if (line[ln_sz - 1] == '\n')
			line[ln_sz - 1] = '\0';
		else if (byts)
			return (byts);

		cmds = parser(cmds, line);
		if (cmds)
		{
			err = isPath(&cmds[0]);
			if (err == 1)
				executor(cmds);
			else if (!err)
			{
				write(STDERR_FILENO, "Command not found\n", 19);
				E_status(127);
			}
			else if (err == -1)
				perror("isPath() failure");

			free_args(cmds);
		}
		free(line);
	}

	return (byts);
}

/**
 * prompt - writes a prompt to the std_out
 */
void prompt(void)
{
	if (write(STDOUT_FILENO, "$!", 2) == -1)
	{
		perror("Whoops");
		exit(E_status(EXIT_FAILURE));
	}
}
