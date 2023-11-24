#include "shell.h"

/**
 * E_status - records an exit status of the program.
 * @stat: the exit status
 *
 * Return: the stored exit status
 */
int E_status(char *prog_name, int stat, char *cmd)
{
	static int err, err_cnt;
	static char *name;
	char *msg = NULL, *stat_s = lintos(stat);

	if (prog_name)
	{
		err_cnt = 1;
		name = prog_name;
	}

	if (stat >= 0)
		err = stat;

	if (stat == 127)
	{
		msg = stringscat(6, name, ": ", stat_s, ": ", cmd, ": not found\n");
		if (msg)
			write(STDOUT_FILENO, msg, _strlen(msg));
		else
		{
			perror("E_status failure");
			exit(EXIT_FAILURE);
		}
		err_cnt++;
	}
	else if (stat == 126)
	{
		msg = stringscat(6, name, ": ", stat_s, ": ", cmd, ": Permission denied\n");
		if (msg)
			write(STDOUT_FILENO, msg, _strlen(msg));
		else
		{
			perror("E_status failure");
			exit(EXIT_FAILURE);
		}
		err_cnt++;
	}

	if (stat_s)
		free(stat_s);

	if (msg)
		free(msg);

	return (err);
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
