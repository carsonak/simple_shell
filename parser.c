#include "shell.h"

/**
 * parser - reads a line from standard input and breaks it into
 * @cmds: address of an array of pointers
 * @line: string to be parsed
 *
 * Return: an array of pointers to the command and options, NULL on failure
 */
char **parser(char **cmds, char *line)
{
	char *token = NULL;
	int i = 1, j = 0;
	cmd_str *head = NULL, *lst_err = NULL;

	token = _strtok(line, " ");
	for (i = 1; token; i++)
	{
		if (token[0])
		{
			lst_err = add_node_end(&head, token);
			j++;
		}

		if (!lst_err)
			return (NULL);

		token = _strtok(NULL, " ");
	}

	if (j)
		cmds = malloc(sizeof(*cmds) * (j + 1));
	else
		return (NULL);

	lst_err = add_node_end(&head, "");
	if (!(cmds))
	{
		perror("parser () memory fail");
		return (NULL);
	}

	cmds = cmds_fill(head, cmds);
	if (!cmds)
		return (NULL);

	free_list(head);
	return (cmds);
}
