#include "shell.h"

/**
 * err_handler - records an exit status of the program.
 * @prog_name: name of this program when called
 * @stat: the exit status
 * @cmd: the command name used
 *
 * Return: the stored exit status
 */
int err_handler(char *prog_name, int stat, char *cmd)
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

	free(cnt_s);
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
		return (0);

	return (1);
}
