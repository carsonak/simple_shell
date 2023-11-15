#include "shell.h"

/**
 * parser - reads a line from standard input and breaks it into
 * @cmds: address of an array of pointers
 *
 * Return: number of commands parsed, -1 on failure
 */
char **parser(char **cmds)
{
	char *str = NULL, *token = NULL;
	int i = 0, idx = 0;
	cmd_str *head = NULL, *lst_err = NULL;

	if (_getline(&str, &idx, STDIN_FILENO) == -1)
	{
		perror("_getline() error");
		return (NULL);
	}

	str[idx - 1] = '\0';
	token = _strtok(str, " ");
	for (i = 1; token; i++)
	{
		lst_err = add_node_end(&head, token);
		if (!lst_err)
			return (NULL);

		token = _strtok(NULL, " ");
	}

	lst_err = add_node_end(&head, "");
	free(str);

	cmds = malloc(sizeof(*cmds) * i);
	if (!(cmds))
	{
		perror("Couldn't allocate memory for commands");
		return (NULL);
	}

	cmds = cmds_fill(head, cmds);
	if (!cmds)
		return (NULL);

	free_list(head);
	return (cmds);
}
