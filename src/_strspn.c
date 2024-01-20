#include "u_string.h"

/**
 * _strspn - checks the first occurence of a sub-string in a string
 * @s: String to be checked
 * @accept: sub-string to check
 *
 * Return: the length of the first occurences
 */
size_t _strspn(char *s, char *accept)
{
	size_t i = 0, j = 0, a = 0;

	if (!s || !accept)
		return (0);

	for (i = 0; s[i]; i++)
	{
		for (j = 0; accept[j]; j++)
			if (accept[j] == s[i])
				break;

		if (accept[j])
			a++;
		else
			break;
	}

	return (a);
}
