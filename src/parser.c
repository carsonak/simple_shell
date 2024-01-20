#include "shell.h"

/**
 * parser - break a string into words and return their pointers in an array
 * @line: string to be parsed
 *
 * Return: an array of pointers, NULL on failure
 */
char **parser(char *line)
{
	char *token = NULL, **cmd = NULL;
	int i = 1, args_i = 0;
	cmd_str *head = NULL;

	if (!line)
		return (NULL);

	token = _strtok(line, " ");
	for (i = 1; token; i++)
	{
		if (token[0])
		{
			if (!add_node_end(&head, token))
			{
				free_list(head);
				return (NULL);
			}

			args_i++;
		}

		token = _strtok(NULL, " ");
	}

	if (args_i && add_node_end(&head, NULL))
		cmd = malloc(sizeof(*cmd) * (args_i + 1));
	else
	{
		free_list(head);
		return (NULL);
	}

	if (!(cmd))
		perror("parser: Malloc fail");

	cmd = cmd_fill(head, cmd);
	free_list(head);
	return (cmd);
}

/**
 * cmd_fill - populate an array of pointers with strings
 * @head: head of a linked list with the strings
 * @cmd: an array of pointers
 *
 * Return: array of pointers, NULL on failure
 */
char **cmd_fill(cmd_str *head, char **cmd)
{
	size_t i = 0, j = 0;
	cmd_str *walk = head;

	if (!cmd)
		return (NULL);

	for (i = 0; walk->next && walk->command; i++)
	{
		cmd[i] = malloc((sizeof(**cmd) * _strlen(walk->command)) + 1);
		if (cmd[i] == NULL)
		{
			free_args(cmd);
			perror("cmd_fill: Malloc failure");
			return (NULL);
		}

		walk = walk->next;
	}

	walk = head;
	for (i = 0; walk->next && walk->command; i++)
	{
		_strcpy(cmd[i], walk->command);
		walk = walk->next;
	}

	cmd[i] = NULL;
	return (cmd);
}
