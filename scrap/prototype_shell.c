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
	char **cmds;

	if (argc == 1)
	{
		/**
		 * This if block should be replaced with a prompting loop that parses
		 * input strings and records the string pointers in an array (cmds).
		 * It then calls (executor) if the command exists.
		 * Should also handle ^C [EOT] and ^D [EOF]
		 */
		error_msg = "Error: Wrong usage.\n./p_sh [path/to/binary/file] [options_to_executable]\n";
		write(STDERR_FILENO, error_msg, (sizeof(*error_msg) * strlen(error_msg)));
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

	/*Forking the processes and recording the PIDs*/
	frk1 = fork();

	if (frk1 == -1)
		perror("Couldn't fork");
	else if (frk1 == 0)
	{
		/**
		 * Replacing the current program with the program or
		 * script specified in cmds[0].
		 */
		err = execve(cmds[0], cmds, nwenviron);
		if (err == -1)
		{
			perror("Couldn't execute");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/*Parent waits for child to terminate and handles any errors*/
		frkerr = waitpid(frk1, &status, 0);
		if (frkerr == -1)
			perror("Wait error");
		else
		{
			/*These dprintf statements should be replace with allowed functions*/
			if (WIFEXITED(status))
				dprintf(STDERR_FILENO, "%d: Exited with status %d\n", frkerr, WEXITSTATUS(status));
			else
				dprintf(STDERR_FILENO, "%d: Exited abnormally", frkerr);
		}
	}

	return (EXIT_SUCCESS);
}
