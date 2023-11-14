#include "shell.h"

/**
 * cmds_fill - allocates memory for the strings in the array
 * @head: pointer to struct with command strings
 * @cmds: array of strings
 *
 * Return: array of pointers
 */
char **cmds_fill(cmd_str *head, char **cmds)
{
	int i = 0, j = 0;
	cmd_str *walk = head;

	for (i = 0; walk->next; i++)
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

	cmds[i] = malloc((sizeof(**cmds) * 1));
	if (cmds[i] == NULL)
	{
		free_args(cmds);
		perror("cmds_fill memory failure");
		return (NULL);
	}

	walk = head;
	for (i = 0; walk->next; i++)
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
