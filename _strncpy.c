#include "shell.h"

/**
 *_strncpy - copies a string with at most n characters to dest.
 *@dest: pointer to a buffer
 *@src: string to be copied
 *@n: max number of characters to be copied.
 *
 * Return: pointer to the result buffer.
 */
char *_strncpy(char *dest, char *src, int n)
{
	long int i, j, l = _strlen(src);

	for (i = 0, j = 0; i < n; i++, j++)
	{
		if (j >= l)
			dest[i] = '\0';
		else
			dest[i] = src[j];
	}

	return (dest);
}
