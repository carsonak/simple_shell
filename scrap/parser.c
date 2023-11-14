#include "shell.h"

/**
 * parser - reads a line from standard input and breaks it into
 * @cmds: address of an array of pointers
 *
 * Return: number of commands parsed, -1 on failure
 */
int parser(char **cmds[])
{
	char *str = NULL, *token = NULL;
	int err = 0, i = 0, ln_idx = 0;
	cmd_str *head = NULL, *walk = NULL, *lst_err = NULL;

	err = _getline(&str, &ln_idx, STDIN_FILENO);
	if (err == -1)
	{
		perror("_getline() error");
		return (-1);
	}

	str[ln_idx - 1] = '\0';
	token = _strtok(str, "\"\"\'\' ");
	for (i = 1; token; i++)
	{
		lst_err = add_node_end(&head, token);
		if (!lst_err)
			return (-1);

		token = _strtok(NULL, "\"\"\'\' ");
	}

	lst_err = add_node_end(&head, "");
	free(str);
	*cmds = malloc(sizeof(**cmds) * i);
	if (!(*cmds))
	{
		perror("parser() error: Couldn't allocate memory for commands");
		return (-1);
	}

	walk = head;
	for (ln_idx = 0; ln_idx < i; ln_idx++)
	{
		*cmds[ln_idx] = walk->command;
		walk = walk->next;
	}

	free_list(head);
	return (i);
}
