#include "scrap.h"

/**
 * parser - reads a line from standard input and breaks it into
 * @cmds: address of an array of pointers
 *
 * Return: number of commands parsed, -1 on failure
 */
int parser(char **cmds[])
{
	char *str = NULL, *token = NULL;
	int err = 0, i = 0, j = 0;
	cmd_str *head = NULL, *walk = NULL, *lst_err = NULL;

	err = _getline(&str, &j, STDIN_FILENO);
	if (err == -1)
	{
		perror("_getline() error");
		return (-1);
	}

	str[j] = '\0';
	token = _strtok(str, "\"\"\'\' ");

	for (i = 1; token; i++)
	{
		lst_err = add_node_end(&head, token);
		if (!lst_err)
			return (-1);

		token = _strtok(NULL, "\"\"\'\' ");
	}

	*cmds = malloc(sizeof(**cmds) * i);
	if (!(*cmds))
	{
		perror("parser() error");
		return (-1);
	}

	walk = head;
	for (j = 0; j <= i; j++)
	{
		*cmds[j] = walk->command;
		walk = walk->next;
	}

	return (i);
}
