#include "simple_shell.h"

#include <stdio.h> /* fprintf */

/**
 * print_func - a function that prints an object.
 * @output_stream: pointer to a stream to print to.
 * @data: the object to print.
 *
 * Return: number of characters printed, negative number on error.
 */
typedef int(print_func)(FILE *output_stream, void const *const data);

/**
 * string_print - prints contents of a string type.
 * @output_stream: pointer to a stream to print to.
 * @data: the string to print.
 *
 * Return: number of characters printed.
 */
int string_print(FILE *output_stream, const void *const data)
{
	const view_string *const s = data;

	if (!s)
		return (fprintf(output_stream, "(null)"));

	return (fprintf(output_stream, "%jd:\"%s\"", s->size, s->s));
}

/**
 * sll_print - print all nodes of a doubly linked list.
 * @output_stream: pointer to a stream to output to.
 * @head: head of the doubly linked list to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: total bytes printed, negative number on error.
 */
long int sll_print(
	FILE *output_stream, single_link_node const *const head,
	print_func *print_data)
{
	long int total_bytes = 0;
	int bytes_printed = 0;
	single_link_node const *walk = head;

	if (!output_stream || !head)
		return (-1);

	if (print_data)
		bytes_printed = print_data(output_stream, walk->data);
	else
		bytes_printed = fprintf(output_stream, "%p", walk->data);

	if (bytes_printed < 0)
		return (bytes_printed);

	total_bytes += bytes_printed;
	walk = walk->next;
	while (walk)
	{
		bytes_printed = fprintf(output_stream, " --> ");
		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		if (print_data)
			bytes_printed = print_data(output_stream, walk->data);
		else
			bytes_printed = fprintf(output_stream, "%p", walk->data);

		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		walk = walk->next;
	}

	bytes_printed = fprintf(output_stream, "\n");
	if (bytes_printed < 0)
		return (bytes_printed);

	return (total_bytes + bytes_printed);
}

/**
 * queue_print - print all nodes of a queue.
 * @output_stream: pointer to the stream to print to.
 * @q: the queue to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: total bytes printed, negative number on error.
 */
long int queue_print(
	FILE *output_stream, const queue *const q, print_func *print_data)
{
	if (!output_stream || !q)
		return (-1);

	if (!q->head)
		return (fprintf(output_stream, "(NULL)\n"));

	return (sll_print(output_stream, q->head, print_data));
}

/**
 * interprate - execute command in tokens.
 * @tokens: list of tokens from input.
 */
void interprate(queue *tokens)
{
	queue_print(stdout, tokens, string_print);
}
