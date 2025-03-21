#include "alloc.h"
#include "strings.h"

/**
 * _malloc - allocates `size` bytes.
 * @size: number of bytes to allocate, cannot be less than 0.
 *
 * Return: pointer to the allocated memory block, NULL on failure.
 */
void *_malloc(const intmax_t size)
{
	if (size < 0)
	{
		perror("ERROR: _malloc");
		return (NULL);
	}

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
void *_calloc(const intmax_t n, const intmax_t type_size)
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
void *_free(void *nullable_ptr)
{
	free(nullable_ptr);
	return (NULL);
}

/**
 * _realloc - resize a memory block to size.
 * @mem: pointer to the memory block.
 * @new_size: size to resize to.
 * @old_size: old size of the memory block.
 *
 * Return: pointer to the resized memory on success, NULL on failure.
 */
void *_realloc(void *mem, const intmax_t new_size, const intmax_t old_size)
{
	void *new_mem = NULL;

	if (new_size < 0)
		return (NULL);

	if (new_size == 0)
		return (_free(mem));

	new_mem = _memcpy(_malloc(new_size), mem, old_size);
	if (!new_mem)
		return (NULL);

	mem = _free(mem);
	return (new_mem);
}

/**
 * _realloc_free_on_fail - resize a memory block to size,
 * free original memory on fail.
 * @mem: pointer to the memory block.
 * @new_size: size in bytes to resize to.
 * @old_size: old size of the memory block.
 *
 * Return: pointer to the resized memory on success, NULL on failure.
 */
void *_realloc_free_on_fail(
	void *mem, const intmax_t new_size, const intmax_t old_size)
{
	void *new_mem = _realloc(mem, new_size, old_size);

	if (!new_mem)
		return (_free(mem));

	return (new_mem);
}
