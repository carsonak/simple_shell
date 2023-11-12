#include "scrap.h"

/**
 * parse - breaks a string into arguments and options
 * @c_s: pointer to a linked list
 * @cmds: address of an array of pointers
 *
 * Return: 0 on success, -1 on failure
 */
int parser(cmd_str *c_s, char **cmds[])
{
	char buffer[BUFFER_SIZE] /*, *str = NULL*/;
	int err = 0, i = 0;
	cmd_str *nw_node = c_s, *walk = nw_node;

	memset(buffer, '\0', BUFFER_SIZE);
	err = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	if (err == -1)
	{
		perror("Failed to read");
		return (-1);
	}

	nw_node = malloc(sizeof(cmd_str));
	if (nw_node)
	{
		nw_node->command = buffer;
		nw_node->next = NULL;
		walk = nw_node;
	}
	else
	{
		perror("Malloc fail in \"parse\"");
		return (-1);
	}

	for (i = 0; buffer[i] && buffer[i + 1] && i < BUFFER_SIZE; i++)
	{
		if (buffer[i] == ' ' && buffer[i + 1] != '"')
		{
			buffer[i] = '\0';
			nw_node = malloc(sizeof(cmd_str));
			if (nw_node)
			{
				walk->next = nw_node;
				nw_node->command = &buffer[i + 1];
				nw_node->next = NULL;
				walk = nw_node;
			}
			else
			{
				free_list(c_s);
				perror("Malloc fail in \"parse\"");
				return (-1);
			}
		}
		else if (buffer[i] == '"')
		{
			buffer[i] = '\0';
			nw_node = malloc(sizeof(cmd_str));
			if (nw_node)
			{
				walk->next = nw_node;
				nw_node->command = &buffer[i + 1];
				nw_node->next = NULL;
				walk = nw_node;
			}
			else
			{
				free_list(c_s);
				perror("Malloc fail in \"parse\"");
				return (-1);
			}

			while (buffer[i] && buffer[i + 1] && (i < BUFFER_SIZE) && (buffer[i] != '"'))
				i++;

			buffer[i] = '\0';
		}
	}

	if (c_s)
		err = cmd_fill(c_s, cmds);

	if (err == -1)
	{
		free_list(c_s);
		return (-1);
	}

	return (0);
}

/**
 * cmd_fill - stores all the pointers in an array
 * @c_s: address of the first node of a linked list
 * @cmds: address of an array of pointers
 *
 * Return: 0 on success, -1 on failure
 */
int cmd_fill(cmd_str *c_s, char **cmds[])
{
	cmd_str *head = c_s, *walk = head;
	unsigned int count = 0, i = 0;

	while (walk)
	{
		count++;
		walk = walk->next;
	}

	walk = head;
	*cmds = malloc(sizeof(**cmds) * (count + 1));
	if (!(*cmds))
	{
		perror("Malloc failure in \"cmd_fill\"");
		return (-1);
	}

	for (i = 0; i < count; i++)
	{
		*cmds[i] = walk->command;
		walk = walk->next;
	}

	return (0);
}

/**
 * free_list - frees space allocated to a linked list
 * @head: address of the first node
 */
void free_list(cmd_str *head)
{
	cmd_str *ptr;

	while (head)
	{
		ptr = head;
		head = head->next;
		free(ptr);
	}

	head = NULL;
}
