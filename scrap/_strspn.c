#include "shell.h"

/**
 *_strspn - checks for the first occurences of a set of characters in a string
 *@s: String to be checked
 *@accept: set of characters to check
 *
 *Return: the length of the first occurences
 */
unsigned int _strspn(char *s, char *accept)
{
	unsigned int i, j, a = 0;

	for (i = 0; s[i]; i++)
	{
		for (j = 0; accept[j]; j++)
		{
			if (accept[j] == s[i])
			{
				a++;
				break;
			}
		}

		if (accept[j] == '\0')
			break;
	}

	return (a);
}
