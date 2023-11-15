#include "shell.h"

/**
 * free_list - frees space allocated to a linked list
 * @head: pointer to the first node
 */
void free_list(cmd_str *head)
{
	cmd_str *ptr;

	while (head != NULL)
	{
		ptr = head;
		head = head->next;
		free(ptr);
	}
}
