#include "u_string.h"

/**
 *_strncat - concatinates 2 strings with at most n characters from src.
 *@dest: first string.
 *@src: second string.
 *@n: max number of characters to be added.
 *
 * Return: pointer to the result string.
 */
char *_strncat(char *dest, char *src, size_t n)
{
	size_t i = 0, j = 0;

	for (i = 0; dest[i]; i++)
		;

	j = 0;
	while (j < n && src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';

	return (dest);
}
