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
 * _memncpy - copies `n` bytes from one memory block to another.
 * @dest: the destination memory.
 * @src: source memory block.
 * @n: number of bytes to copy.
 *
 * Return: returns pointer to `dest`.
 */
char *_memncpy(char *const dest, const char *const src, const intmax_t n)
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
 * _strlen - counts number of characters in a string upto
 * the terminating null byte.
 * @s: pointer to the string.
 *
 * Return: length of the string.
 */
size_t _strlen(const char *const s)
{
	size_t i;

	if (!s)
		return (0);

	for (i = 0; s[i]; ++i)
		;

	return (i);
}

/**
 * _strncmp - compares the first `len` characters of two strings.
 * @s1: pointer to the first string.
 * @s2: pointer to the second string.
 * @len: number of characters to compare, cannot be less than 0.
 *
 * Return: 0 if strings are equal,
 * the difference of the first differing characters otherwise.
 */
short int _strncmp(const char *s1, const char *s2, intmax_t len)
{
	if (!s1 || !s2 || len < 1)
		return (0);

	while (len > 0 && !(*s1 - *s2))
	{
		++s1;
		++s2;
		--len;
	}

	return (*s1 - *s2);
}
