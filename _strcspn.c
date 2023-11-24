#include "shell.h"

/**
 * _strcspn - checks the first occurence of a sub-string in a string
 * @s: String to be checked
 * @reject: sub-string to check
 *
 * Return: the length of the first non-occurences
 */
size_t _strcspn(char *s, char *reject)
{
	size_t i, j, r = 0;

	for (i = 0; s[i]; i++)
	{
		for (j = 0; reject[j]; j++)
			if (reject[j] == s[i])
				break;

		if (!reject[j])
			r++;
		else
			break;
	}

	return (r);
}
