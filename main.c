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
	if (argc == 1)
	{
		while (1)
		{
			errno = 0;
			if (isatty(STDIN_FILENO))
				prompt();

			parse_n_exec();
		}
	}
	else
	{
		cmds = &argv[1];
		err = isPath(&cmds[0]);
		if (err == 1)
			executor(cmds);
		else if (!err)
			write(STDERR_FILENO, "Command not found\n", 19);
		else if (err == -1)
			perror("isPath() failure");
	}
	exit(EXIT_SUCCESS);
}

/**
 * parse_n_exec - parses std input and executes the commands
 */
void parse_n_exec(void)
{
	char **cmds = NULL, *line = NULL;
	ssize_t err = 0, ln_sz = 0;

	while (1)
	{
		errno = 0;
		line = NULL;
		err = _getline(&line, &ln_sz, STDIN_FILENO);
		if (ln_sz > 0)
		{
			if (ln_sz == 1 && line[0] == '\n')
			{
				free(line);
				return;
			}

			if (line[ln_sz - 1] == '\n')
				line[ln_sz - 1] = '\0';

			cmds = parser(cmds, line);
			if (cmds)
			{
				err = isPath(&cmds[0]);
				flush_io();
				if (err == 1)
					executor(cmds);
				else if (!err)
					write(STDERR_FILENO, "Command not found\n", 19);
				else if (err == -1)
					perror("isPath() failure");

				free_args(cmds);
			}
		}
		else if (err < 0)
			perror("_getline() error");
		else if (!err && !ln_sz)
			break;
	}
}

/**
 * prompt - writes a prompt to the std_out
 */
void prompt(void)
{
	if (write(STDOUT_FILENO, "$!", 2) == -1)
	{
		perror("Whoops");
		exit(EXIT_FAILURE);
	}
}
