#include "queue.h"

/**
 * queue_new - allocate memory for an empty queue.
 *
 * Return: pointer to the new queue, NULL on failure.
 */
queue *queue_new(void) { return (_calloc(1, sizeof(queue))); }

/**
 * enqueue - add a node to the end of a queue.
 * @q: the queue to operate on.
 * @data: data that the node will hold.
 * @copy_data: function that returns a separate copy of data,
 * if NULL a simple copy of the pointer to data is done.
 *
 * Return: pointer to the newly added node, NULL if q is NULL or failure.
 */
single_link_node *enqueue(
	queue *const q, void *const data, dup_func *copy_data)
{
	single_link_node *nw = NULL;

	if (!q)
		return (NULL);

	nw = sln_new(data, copy_data);
	if (!nw)
		return (NULL);

	q->tail = sln_insert_after(q->tail, nw);
	if (!q->head)
		q->head = nw;

	++(q->length);
	return (nw);
}

/**
 * dequeue - pop a node from the start of a queue and return its data.
 * @q: the queue to operate on.
 *
 * Return: pointer to the data in the popped node, NULL if q or head is NULL.
 */
void *dequeue(queue *const q)
{
	single_link_node *node = NULL;
	void *d = NULL;

	if (!q || !q->head)
		return (NULL);

	node = q->head;
	q->head = node->next;
	d = sln_remove(node);
	if (!q->head)
		q->tail = NULL;

	if (q->length)
		--(q->length);

	return (d);
}

/**
 * clear_queue - delete all items in a queue.
 * @q: the queue to operate on.
 * @free_data: pointer to a function that will be called to free data in nodes.
 */
static void clear_queue(queue *const q, delete_func *free_data)
{
	if (!q)
		return;

	q->head = sll_clear(q->head, free_data);
	q->tail = NULL;
	q->length = 0;
}

/**
 * queue_delete - frees a queue from memory.
 * @nullable_ptr: pointer to the queue to delete.
 * @free_data: pointer to a function that can free data in the queue.
 *
 * Return: NULL always.
 */
void *queue_delete(queue *const nullable_ptr, delete_func *free_data)
{
	clear_queue(nullable_ptr, free_data);
	return (_free(nullable_ptr));
}
