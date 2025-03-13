#ifndef SIMPLE_SHELL_SINGLY_LINKED_LIST_H
#define SIMPLE_SHELL_SINGLY_LINKED_LIST_H

#include "alloc.h"

/**
 * struct single_link_node - a singly linked list node.
 * @data: data that the node holds.
 * @next: pointer to the next node.
 * @previous: pointer to the previous node.
 */
struct single_link_node
{
	void *data;
	struct single_link_node *next;
	/*This second pointer makes insertions and deletions easier.*/
	struct single_link_node *previous;
};

typedef struct single_link_node single_link_node;

single_link_node *sln_new(void *const data, dup_func *duplicate_data);
single_link_node *sln_insert_after(
	single_link_node *const this_node, single_link_node *const other_node);
single_link_node *sln_insert_before(
	single_link_node *const this_node, single_link_node *other_node);
void *sln_remove(single_link_node *const node);
void *sln_swap_data(
	single_link_node *const node, void *const data, dup_func *copy_data);
void *sll_clear(single_link_node *const head, delete_func *free_data);

#endif /* SIMPLE_SHELL_SINGLY_LINKED_LIST_H */
