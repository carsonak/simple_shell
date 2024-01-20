#include "u_string.h"

/**
 *_strlen - calculates length of a string
 *@s: the string
 *
 *Return: length of the string
 */
size_t _strlen(char *s)
{
	size_t cnt = 0;

	for (cnt = 0; s && s[cnt]; cnt++)
		;

	return (cnt);
}
