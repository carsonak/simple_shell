#include "shell.h"

/**
 * free_args - frees a 2D array
 * @cmds: a 2D array (the command and it's options)
 */
void free_args(char **cmds)
{
	int i = 0;

	for (i = 0; cmds[i]; i++)
		free(cmds[i]);

	free(cmds[i]);
	free(cmds);
}
