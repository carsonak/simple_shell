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
	E_status(argv[0], EXIT_SUCCESS, NULL);
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
			E_status(NULL, 127, cmds[0]);
		else if (err == -1)
			E_status(NULL, 126, cmds[0]);
	}

	return (E_status(NULL, -1, NULL));
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
			return (ln_sz);
		}

		if (line[ln_sz - 1] == '\n')
			line[ln_sz - 1] = '\0';

		cmds = parser(cmds, line);
		if (cmds)
		{
			err = isPath(&cmds[0]);
			if (err == 1)
				executor(cmds);
			else if (!err)
				E_status(NULL, 127, cmds[0]);
			else if (err == -1)
				E_status(NULL, 126, cmds[0]);

			free_args(cmds);
		}
		free(line);
	}

	return (ln_sz);
}

/**
 * prompt - writes a prompt to the std_out
 */
void prompt(void)
{
	if (write(STDOUT_FILENO, "$!", 2) == -1)
	{
		perror("Whoops");
		exit(E_status(NULL, EXIT_FAILURE, NULL));
	}
}
