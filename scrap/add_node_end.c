#include "shell.h"

/**
 * add_node_end - appends a new node at the end of a linked list
 * @head: address of the pointer to first node
 * @str: String to be appended to the list
 *
 * Return: pointer to first element in the new list
 */
cmd_str *add_node_end(cmd_str **head, char *str)
{
	cmd_str *new_node, *temp;

	if (!str)
	{
		write(STDERR_FILENO, "add_node_end() error: string is empty\n", 39);
		return (NULL);
	}

	new_node = malloc(sizeof(cmd_str));
	if (!new_node)
	{
		perror("add_node_end() error! Couldn't allocate memory");
		return (NULL);
	}
	if (_strlen(str))
	{
		new_node->command = _strdup(str);
		if (!(new_node->command))
		{
			perror("add_node_end() error! Couldn't duplicate string");
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->command = NULL;

	new_node->len = _strlen(str);
	new_node->next = NULL;
	if (!(*head))
	{
		*head = new_node;
		return (new_node);
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;

	temp->next = new_node;
	return (new_node);
}
