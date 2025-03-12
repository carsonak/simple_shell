#ifndef SIMPLE_SHELL_QUEUE_H
#define SIMPLE_SHELL_QUEUE_H

#include <stddef.h> /* size_t */

#include "alloc.h"
#include "singly_linked_list.h"

/**
 * struct queue - a queue data structure.
 * @length: number of items in the queue.
 * @head: a pointer to the head of the queue.
 * @tail: a pointer to the tail of the queue.
 */
struct queue
{
	size_t length;
	single_link_node *head;
	single_link_node *tail;
};

typedef struct queue queue;

queue *queue_new(void);
void *queue_delete(queue *const nullable_ptr, delete_func *free_data);
single_link_node *enqueue(
	queue *const q, void *const data, dup_func *copy_data);
void *dequeue(queue *const q);
void **queue_to_array(
	const queue *const q, dup_func *copy_data, delete_func *free_data);

#endif /* SIMPLE_SHELL_QUEUE_H */
