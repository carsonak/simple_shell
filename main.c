#include "shell.h"

/**
 * main - entry point
 * @argc: number of arguments passed to the program
 * @argv: the arguments passed
 *
 * Return: always 0
 */
int main(int argc, char *argv[])
{
	char **cl_args = NULL;
	int err = 0;

	errno = 0;
	err_handler(argv[0], EXIT_SUCCESS, NULL);
	if (argc == 1)
	{
		if (isatty(STDIN_FILENO))
			inatty();
		else
			notatty();
	}
	else
	{
		cl_args = &argv[1];
		err = isPath(cl_args);
		if (err == 1)
			executor(cl_args);
		else if (err == 0)
			err_handler(NULL, 127, cl_args[0]);
	}

	return (err_handler(NULL, -1, NULL));
}

/**
 * inatty - The shell is reading input from a terminal
 */
void inatty(void)
{
	char **cmd_arr = NULL, *line = NULL, *cmd_cpy = NULL;
	ssize_t err = 0, ln_sz = 0, len = 1;

	while (len)
	{
		errno = 0;
		prompt();
		len = _getline(&line, &ln_sz, STDIN_FILENO);
		if (len == -1)
			perror("_getline() error");
		else if (len > 0)
		{
			cmd_arr = parser(line);
			if (cmd_arr)
			{
				cmd_cpy = cmd_arr[0];
				err = isPath(&cmd_arr[0]);
				if (err == 1)
				{
					executor(cmd_arr);
					free(cmd_cpy);
					cmd_cpy = NULL;
				}
				else if (err == 0)
					err_handler(NULL, 127, cmd_arr[0]);

				free_args(cmd_arr);
			}

			free(line);
			line = NULL;
		}
	}

	write(STDOUT_FILENO, "\n", 1);
}

/**
 * notatty - std-input is not from terminal
 */
void notatty(void)
{
	char **cmd_arr = NULL, *line = NULL, *cmd_cpy = NULL;
	ssize_t err = 0, ln_sz = 0, len = 1;

	while (len)
	{
		errno = 0;
		len = _getline(&line, &ln_sz, STDIN_FILENO);
		if (len == -1)
			perror("_getline() error");
		else if (len > 0)
		{
			cmd_arr = parser(line);
			if (cmd_arr)
			{
				cmd_cpy = cmd_arr[0];
				err = isPath(&cmd_arr[0]);
				if (err == 1)
				{
					executor(cmd_arr);
					free(cmd_cpy);
					cmd_cpy = NULL;
				}
				else if (err == 0)
					err_handler(NULL, 127, cmd_arr[0]);

				free_args(cmd_arr);
			}

			free(line);
			line = NULL;
		}
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
		exit(err_handler(NULL, EXIT_FAILURE, NULL));
	}
}
