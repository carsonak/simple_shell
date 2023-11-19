#include "shell.h"

/**
 * _strncmp - compares the first n characters of 2 strings
 * @s1: first string.
 * @s2: second string.
 * @n: the maximum nuber of characters to be checked
 *
 *Return: 0 if both equal, difference in their ASCII values on first mismatch
 */
int _strncmp(char *s1, char *s2, unsigned int n)
{
	unsigned int i = 0;

	for (i = 0; i < n; i++)
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);

	return (0);
}
