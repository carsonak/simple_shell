#include "shell.h"

/**
 * _strdup - duplicates a string (allocates new memory for the copy)
 * @str: the string
 *
 * Return: a pointer to the new copy, NULL if it fails.
 */
char *_strdup(char *str)
{
	char *cpy = NULL;
	unsigned long int i = 0;

	if (!str)
		return (NULL);

	cpy = malloc((sizeof(*str) * _strlen(str)) + 1);
	if (!cpy)
		return (NULL);

	for (i = 0; i <= _strlen(str); i++)
		cpy[i] = str[i];

	return (cpy);
}
