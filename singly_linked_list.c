#include "singly_linked_list.h"

/**
 * sln_new - allocates and initialises memory for a `single_link_node`.
 * @data: pointer to the object to initialise with.
 * @duplicate_data: function that returns a separate copy of data,
 * if NULL the pointer to the original data is stored.
 *
 * Return: pointer to the new node, NULL on failure.
 */
single_link_node *sln_new(void *const data, dup_func *duplicate_data)
{
	single_link_node *new_node = _calloc(1, sizeof(*new_node));

	if (!new_node)
		return (NULL);

	new_node->data = data;
	if (duplicate_data)
	{
		new_node->data = duplicate_data(data);
		if (!new_node->data && data)
			return (_free(new_node));
	}

	return (new_node);
}

/**
 * sln_insert_after - inserts a `single_link_node` after another.
 * @this_node: the node to insert after.
 * @other_node: the node to insert.
 *
 * Return: pointer to the newly inserted node.
 */
single_link_node *sln_insert_after(
	single_link_node *const this_node, single_link_node *const other_node)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (this_node);

	other_node->previous = this_node;
	other_node->next = this_node->next;
	if (this_node->next)
		this_node->next->previous = other_node;

	this_node->next = other_node;
	return (other_node);
}

/**
 * sln_insert_before - insert a single link node before another.
 * @this_node: the node to insert before.
 * @other_node: the node to insert.
 *
 * Return: pointer to the newly inserted node.
 */
single_link_node *sln_insert_before(
	single_link_node *const this_node, single_link_node *other_node)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (this_node);

	other_node->next = this_node;
	other_node->previous = this_node->previous;
	if (this_node->previous)
		this_node->previous->next = other_node;

	this_node->previous = other_node;
	return (other_node);
}

/**
 * sln_remove - deletes a node and returns its data.
 * @node: the node to delete.
 *
 * Return: pointer to the data of the node.
 */
void *sln_remove(single_link_node *const node)
{
	void *d = NULL;

	if (!node)
		return (NULL);

	d = node->data;
	node->data = NULL;
	if (node->next)
		node->next->previous = node->previous;

	if (node->previous)
		node->previous->next = node->next;

	node->next = NULL;
	node->previous = NULL;
	_free(node);
	return (d);
}

/**
 * sln_swap_data - replace data in a node.
 * @node: pointer to the node to modify.
 * @data: pointer to the data to swap in.
 * @copy_data: function that will be called to duplicate data, if NULL
 * only pointer to `data` is stored.
 *
 * Return: pointer to the old data in node, NULL on failure.
 */
void *sln_swap_data(
	single_link_node *const node, void *const data, dup_func *copy_data)
{
	void *old_data = NULL;

	if (!node)
		return (NULL);

	old_data = node->data;
	node->data = data;
	if (copy_data)
	{
		node->data = copy_data(data);
		if (!node->data && data)
		{
			node->data = old_data;
			return (NULL);
		}
	}

	return (old_data);
}

/**
 * sll_clear - delete a doubly linked list from memory.
 * @head: pointer to the head of the doubly linked list.
 * @free_data: function that will be called to free data in the nodes.
 *
 * Return: NULL always.
 */
void *sll_clear(single_link_node *const head, delete_func *free_data)
{
	single_link_node *walk = head;
	void *data = NULL;

	if (!head)
		return (NULL);

	while (walk->next)
	{
		walk = walk->next;
		data = sln_remove(walk->previous);
		if (free_data)
			free_data(data);
	}

	data = sln_remove(walk);
	if (free_data)
		free_data(data);

	return (NULL);
}
