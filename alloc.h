#ifndef SIMPLE_SHELL_ALLOC_H
#define SIMPLE_SHELL_ALLOC_H

#include <stdlib.h>   /* malloc */
#include <stdio.h>	  /* perror */
#include <inttypes.h> /* intmax_t */

/**
 * dup_func - a function that duplicates an object.
 * @data: pointer to the object to be duplicated.
 *
 * Return: pointer to the duplicate, NULL on failure.
 */
typedef void *(dup_func)(void const *const data);

/**
 * delete_func - a function that deletes an object.
 * @data: pointer to the object to delete.
 */
typedef void(delete_func)(void *const data);

void *_malloc(const intmax_t size);
void *_calloc(const intmax_t n, const intmax_t type_size);
void *_free(void *nullable_ptr);
void *_realloc(void *mem, const intmax_t size);
void *_realloc_free_on_fail(void *mem, const intmax_t size);
void *delete_2D_array(
	void **array, intmax_t size, delete_func *free_row);
void **dup_2D_array(
	void *const *const array, const intmax_t size,
	dup_func *copy_data, delete_func *free_data);

#endif /* SIMPLE_SHELL_ALLOC_H */
