#include "shell.h"

/**
 *_strlen - calculates length of a string
 *@s: the string
 *
 *Return: length of the string
 */
size_t _strlen(char *s)
{
	size_t cnt;

	for (cnt = 0; s[cnt]; cnt++)
		;

	return (cnt);
}
