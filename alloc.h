#ifndef SIMPLE_SHELL_ALLOC_H
#define SIMPLE_SHELL_ALLOC_H

#include <stdlib.h>   /* malloc */
#include <inttypes.h> /* intmax_t */

/**
 * _malloc - allocates `size` bytes.
 * @size: number of bytes to allocate, cannot be less than 0.
 *
 * Return: pointer to the allocated memory block, NULL on failure.
 */
void *_malloc(intmax_t size);
void *_calloc(intmax_t n, intmax_t type_size);
void *_free(const void *nullable_ptr);

#endif /* SIMPLE_SHELL_ALLOC_H */
