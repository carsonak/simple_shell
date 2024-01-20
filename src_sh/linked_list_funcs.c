#include "shell.h"

/**
 * add_node_end - appends a new node at the end of a linked list
 * @head: address of the pointer to first node of the list
 * @str: data to be stored
 *
 * Return: pointer to to the new node, NULL on failure
 */
cmd_str *add_node_end(cmd_str **head, char *str)
{
	cmd_str *new_node = NULL, *temp = NULL;

	if (!head)
		return (NULL);

	new_node = malloc(sizeof(*new_node));
	if (!new_node)
	{
		perror("add_node_end(): Malloc fail");
		return (NULL);
	}

	if (str && str[0])
	{
		new_node->command = _strdup(str);
		if (!(new_node->command))
		{
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->command = NULL;

	new_node->len = _strlen(str);
	new_node->next = NULL;
	if (!(*head))
		*head = new_node;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;

		temp->next = new_node;
	}

	return (new_node);
}

/**
 * free_list - frees space allocated to a linked list
 * @head: pointer to the first node
 */
void free_list(cmd_str *head)
{
	cmd_str *ptr = NULL;

	while (head)
	{
		ptr = head;
		head = head->next;
		free(ptr->command);
		free(ptr);
	}
}
