#include "shell.h"

/**
 * E_status - records an exit status of the program.
 * @prog_name: name of this program when called
 * @stat: the exit status
 * @cmd: the command name used
 *
 * Return: the stored exit status
 */
int E_status(char *prog_name, int stat, char *cmd)
{
	static int err, err_cnt;
	static char *name;
	char *panic = NULL, *msg = NULL, *cnt_s = NULL;

	if (prog_name)
	{
		err_cnt = 1;
		name = prog_name;
	}

	if (stat >= 0)
		err = stat;

	if (stat == 127)
	{
		panic = ": not found\n";
		cnt_s = lintos(err_cnt);
		if (!badbad(name, &msg, cnt_s, cmd, panic))
			exit(EXIT_FAILURE);

		err_cnt++;
	}
	else if (stat == 126)
	{
		panic = ": Permission denied\n";
		cnt_s = lintos(err_cnt);
		if (!badbad(name, &msg, cnt_s, cmd, panic))
			exit(EXIT_FAILURE);

		err_cnt++;
	}

	if (cnt_s)
		free(cnt_s);

	if (msg)
		free(msg);

	return (err);
}

/**
 * badbad - prints out an error message to standard output
 * @prog_name: name of this program when called
 * @msg: address of a buffer to store the error message
 * @cnt_s: current number of errors encoutered during runtime as a string
 * @cmd: command invoked when error was found
 * @panic: description of the error
 *
 * Return: 1 on success, 0 on failure
 */
int badbad(char *prog_name, char **msg, char *cnt_s, char *cmd, char *panic)
{
	*msg = stringscat(6, prog_name, ": ", cnt_s, ": ", cmd, panic);
	if (*msg)
		write(STDERR_FILENO, *msg, _strlen(*msg));
	else
	{
		perror("E_status failure");
		return (0);
	}

	return (1);
}

/**
 * lintos - converts a long int to a string
 * @num: a long int
 *
 * Return: pointer to the string, NULL on failure
 */
char *lintos(ssize_t num)
{
	ssize_t div = 1, idx = 0, val = num, i = 0;
	char *num_str = NULL;

	if (num < 0)
	{
		num = -val;
		idx++;
	}

	while (val / div >= 10)
	{
		div *= 10;
		idx++;
	}

	num_str = malloc(sizeof(*num_str) * (idx + 2));
	if (num_str)
	{
		if (num < 0)
		{
			num_str[0] = '-';
			i++;
		}

		while (i <= idx && div)
		{
			num_str[i] = ((val / div) + '0');
			val %= div;
			div /= 10;
			i++;
		}

		num_str[i] = '\0';
	}

	return (num_str);
}
