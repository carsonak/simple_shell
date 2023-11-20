#include "shell.h"

/**
 * cmds_fill - allocates memory for the strings in the array
 * @head: pointer to struct with command strings
 * @cmds: an array of pointers
 *
 * Return: array of pointers, NULL on failure
 */
char **cmds_fill(cmd_str *head, char **cmds)
{
	int i = 0, j = 0;
	cmd_str *walk = head;

	for (i = 0; walk->next && walk->command[0]; i++)
	{
		cmds[i] = malloc((sizeof(**cmds) * _strlen(walk->command)) + 1);
		if (cmds[i] == NULL)
		{
			free_args(cmds);
			perror("cmds_fill memory failure");
			return (NULL);
		}
		walk = walk->next;
	}

	walk = head;
	for (i = 0; walk->next && walk->command[0]; i++)
	{
		for (j = 0; walk->command[j]; j++)
			cmds[i][j] = walk->command[j];

		cmds[i][j] = '\0';
		free(walk->command);
		walk = walk->next;
	}
	cmds[i] = NULL;

	return (cmds);
}
