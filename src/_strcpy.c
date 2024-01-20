#include "u_string.h"

/**
 * _strcpy - copies a string to dest.
 * @dest: a buffer to hold the string
 * @src: string to be copied
 *
 * Return: pointer to the copied string
 */
char *_strcpy(char *dest, char *src)
{
	size_t i = 0;

	for (i = 0; src[i]; i++)
		dest[i] = src[i];

	dest[i] = '\0';
	return (dest);
}
