#include "u_string.h"

/**
 * _strdup - duplicates a string (allocates new memory for the copy)
 * @str: the string
 *
 * Return: a pointer to the new copy, NULL if it fails.
 */
char *_strdup(char *str)
{
	char *cpy = NULL;
	size_t i = 0, len = 0;

	if (!str)
		return (NULL);

	len = _strlen(str);
	cpy = malloc((sizeof(*str) * len) + 1);
	if (!cpy)
	{
		perror("_strdup: malloc fail");
		return (NULL);
	}

	for (i = 0; i <= len; i++)
		cpy[i] = str[i];

	return (cpy);
}
