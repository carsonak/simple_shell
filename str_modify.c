#include "strings.h"

/**
 * overlaps_greater_indices - checks if a block of memory overlaps
 * another on its higher indices.
 * @mem1: potentially overlapping memory block..
 * @mem2: other memory block.
 * @bytes: size of the memory blocks.
 *
 * Return: 1 if mem1 overlaps mem2's higher indices, 0 otherwise.
 */
static unsigned char overlaps_greater_indices(
	const char *const mem1, const char *const mem2, const intmax_t bytes)
{
	return (mem1 == mem2 || (mem1 > mem2 && mem1 < (mem2 + bytes)));
}

/**
 * _memcpy - copies `n` bytes from one memory block to another.
 * @dest: the destination memory.
 * @src: source memory block.
 * @n: number of bytes to copy.
 *
 * Return: returns pointer to `dest`.
 */
char *_memcpy(char *const dest, const char *const src, const intmax_t n)
{
	intmax_t i;

	if (!dest || !src || n <= 0)
		return (dest);

	if (overlaps_greater_indices(dest, src, n) == 1)
	{
		for (i = n; i > 0; --i)
			dest[i - 1] = src[i - 1];
	}
	else
	{
		for (i = 0; i < n; ++i)
			dest[i] = src[i];
	}

	return (dest);
}

/**
 * _memset - fills a memory block with a constant byte.
 * @mem: pointer to the memory block.
 * @byte: byte to fill block with.
 * @size: size in bytes to fill.
 *
 * Return: pointer to the modified memory block.
 */
char *_memset(char *const mem, const char byte, intmax_t size)
{
	if (!mem || size <= 0)
		return (mem);

	while (size > 0)
	{
		--size;
		mem[size] = byte;
	}

	return (mem);
}

/**
 * _strdup - duplicate a string in memory.
 * @str: the string to duplicate.
 * @size: size of the string.
 *
 * Return: pointer to the duplicated string, NULL on error.
 */
char *_strdup(const char *const str, intmax_t size)
{
	char *new_str = NULL;

	if (!str || size < 1)
		return (NULL);

	new_str = _memcpy(_malloc(sizeof(*str) * size + 1), str, size);
	new_str[size] = '\0';
	return (new_str);
}

/**
 * _strncat - concatenates two strings by appending at most `n` bytes of `s`
 * to the end of `dest` and then terminates with a '\0'.
 * @dest: pointer to a memory block with the first string and enough space for
 * atleast `n` + 1 bytes of the second.
 * @s: the second string.
 * @n: maximum number of bytes from `s` to append.
 *
 * Return: pointer to the concatenated strings, NULL if both pointers are NULL.
 */
char *_strncat(char *dest, const char *const s, intmax_t n)
{
	intmax_t dest_len = _strlen(dest);
	intmax_t s_len = _strlen(s);

	if (!dest && !s)
		return (NULL);

	if (dest_len < 1)
		return ((char *)s);

	if (s_len < 1 || n < 1)
		return (dest);

	if (s_len > n)
		s_len = n;

	dest = _memcpy(dest + dest_len, s, s_len);
	dest[dest_len + s_len] = '\0';
	return (dest);
}
