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
	char *error_msg = NULL;

	if (argc == 1)
	{
		/*This if block should be replaced with a prompting loop*/
		error_msg = "Error: Wrong usage.\nUse: ./p_sh [command] [options]\n";
		write(STDERR_FILENO, error_msg, (sizeof(*error_msg) * strlen(error_msg)));
	}
	else
	{
		/**
		 * Should be able to detrmine if command exist via searching
		 * the PATH environment variable before calling the executor.
		 * */
		executor(argv);
	}

	return (0);
}

/**
 * executor - forks the process and executes given command
 * @cmds: an array of pointers to the command and its options
 *
 * Return: standard success int or standard failure int
 */
int executor(char **cmds)
{
	pid_t frk1 = 0, frkerr = 0;
	int err = 0, status;
	char **nwenviron = __environ, *error_msg = NULL;

	/*Forking the processes and recording the PIDs*/
	frk1 = fork();

	if (frk1 == -1)
		perror("Couldn't fork");
	else if (frk1 == 0)
	{
		/**
		 * Replacing the current program with the program or
		 * script specified in cmds[0].
		 * */
		err = execve(cmds[0], cmds, nwenviron);
		if (err == -1)
		{
			perror("\nCouldn't execute");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/*Parent waits for child to terminate and handles any errors*/
		frkerr = waitpid(frk1, &status, WNOHANG);
		if (frkerr == -1)
			perror("\nWait error");
		else if (frkerr > 0)
		{
			/*These dprintf statements should be replace with allowed functions*/
			if (WIFEXITED(status))
				dprintf(STDERR_FILENO, "%d: Exited with %d status\n", frkerr, WEXITSTATUS(status));
			else
				dprintf(STDERR_FILENO, "%d: Exited abnormally", frkerr);
		}
		else
		{
			error_msg = "No children\n";
			write(STDERR_FILENO, error_msg, (sizeof(*error_msg) * strlen(error_msg)));
		}
	}

	return (EXIT_SUCCESS);
}
