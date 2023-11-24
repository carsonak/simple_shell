#include "shell.h"

/**
 * executor - forks the process and executes given command in child process
 * @cmds: an array of pointers to the command and its options
 *
 * Return: standard success int or standard failure int
 */
int executor(char **cmds)
{
	pid_t frk1 = 0, frkerr = 0;
	int err = 0, status;
	char **nwenviron = __environ;

	frk1 = fork();

	if (frk1 == -1)
		perror("Couldn't fork");
	else if (frk1 == 0)
	{
		err = execve(cmds[0], cmds, nwenviron);
		if (err == -1)
			exit(E_status(NULL, EXIT_FAILURE, NULL));
	}
	else
	{
		frkerr = waitpid(frk1, &status, 0);
		if (frkerr == -1)
			perror("Wait error");
		else
			return (E_status(NULL, WEXITSTATUS(status), cmds[0]));
	}

	return (E_status(NULL, EXIT_SUCCESS, NULL));
}
