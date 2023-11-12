#include "scrap.h"

/**
 *_memset - fills a memory with a constant byte.
 *@s: pointer to the string
 *@c: the character to be used
 *@n: number of times the character will be added
 *
 *Return: pointer to the string
 */

char *_memset(char *s, char c, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		s[i] = c;

	return (s);
}
