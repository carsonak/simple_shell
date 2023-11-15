#include "shell.h"

/**
 * exit : function that terminate the prog when user type "exit"
 * @cmd :string to read
 * return:  nothing.
 **/
void exit(char *cmd)
{
	if (_strcmp("exit", cmd) == 0)
	{
		free(cmd);
		exit(EXIT_SUCCESS);
	}
}

