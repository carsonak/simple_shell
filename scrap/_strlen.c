#include "shell.h"

/**
 *_strlen - calculates length of a string
 *@s: the string
 *
 *Return: length of the string
 */
unsigned long int _strlen(char *s)
{
	unsigned long int cnt;

	for (cnt = 0; s[cnt]; cnt++)
		;

	return (cnt);
}
