#include "shell.h"

/**
 *
 */
char **cmds_fill(cmd_str *head, char **cmds)
{
	int i = 0, j = 0;
	cmd_str *walk = head;

	for (j = 0; walk->next; j++)
	{
		cmds[j] = malloc((sizeof(**cmds) * _strlen(walk->command)) + 1);
		if (cmds[j] == NULL)
		{
			perror("cmds_fill memory failure");
			return (NULL);
		}
		walk = walk->next;
	}

	cmds[j] = malloc((sizeof(**cmds) * 1));
	if (cmds[j] == NULL)
	{
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
