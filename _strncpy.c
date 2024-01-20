#include "u_string.h"

/**
 *_strncpy - copies a string with at most n characters to dest.
 *@dest: pointer to a buffer
 *@src: string to be copied
 *@n: max number of characters to be copied.
 *
 * Return: pointer to the result buffer.
 */
char *_strncpy(char *dest, char *src, size_t n)
{
	size_t i, l = _strlen(src);

	for (i = 0; i < n; i++)
	{
		if (i >= l)
			dest[i] = '\0';
		else
			dest[i] = src[i];
	}

	return (dest);
}
