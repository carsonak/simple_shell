#include "alloc.h"
#include "string.h"

/**
 * _malloc - allocates `size` bytes.
 * @size: number of bytes to allocate, cannot be less than 0.
 *
 * Return: pointer to the allocated memory block, NULL on failure.
 */
void *_malloc(intmax_t size)
{
	if (size < 0)
		return (NULL);

	return (malloc(size));
}

/**
 * _calloc - allocates memory for an array of `n` elements of `type_size` bytes
 * each and initialises all elements to 0.
 * @n: number of elements in the array, cannot be less than 0.
 * @type_size: size in bytes of each element, cannot be less than 0.
 *
 * Return: pointer to the allocated memory block, NULL on failure.
 */
void *_calloc(intmax_t n, intmax_t type_size)
{
	if (n < 0 || type_size < 0)
		return (NULL);

	return (_memset(_malloc(n * type_size), '\0', n * type_size));
}

/**
 * _free - deallocates a memory block.
 * @nullable_ptr: pointer to the memory block to free.
 *
 * Return: NULL always.
 */
void *_free(const void *nullable_ptr)
{
	free((void *)nullable_ptr);
	return (NULL);
}
