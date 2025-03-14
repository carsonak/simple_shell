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
 * queue_clear - delete all items in a queue.
 * @q: the queue to operate on.
 * @free_data: pointer to a function that will be called to free data in nodes.
 */
void queue_clear(queue *const q, delete_func *free_data)
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
	queue_clear(nullable_ptr, free_data);
	return (_free(nullable_ptr));
}

/**
 * queue_to_array - create an array from a queue.
 * @q: the queue.
 * @copy_data: optional pointer to a function that will be used to duplicate
 * the data, if not provided, array will contain pointers to the original data
 * in the queue.
 * @free_data: optional pointer to a function that will be used to free data in
 * the array in case of failure. If `copy_data` is provided this function must
 * also be provided, otherwise no data duplication will occur.
 *
 * Return: pointer to the data array on success, NULL on failure.
 */
void **queue_to_array(
	const queue *const q, dup_func *copy_data, delete_func *free_data)
{
	void **data_array = NULL;
	single_link_node *node = NULL;
	intmax_t d_i = 0;

	if (!q || !q->head || q->length < 1)
		return (NULL);

	data_array = _calloc(q->length + 1, sizeof(*data_array));
	if (!data_array)
		return (NULL);

	node = q->head;
	for (d_i = 0; node; node = node->next, ++d_i)
	{
		data_array[d_i] = node->data;
		if (copy_data && free_data)
		{
			data_array[d_i] = copy_data(node->data);
			if (!data_array[d_i] && node->data)
				return (delete_2D_array(data_array, q->length, free_data));
		}
	}

	return (data_array);
}
