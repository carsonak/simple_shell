#include "u_string.h"

/**
 * pad_char - calculates length of initial padding characters in a string
 * @str: the string to check
 * @ch: the character
 *
 * Return: number of padding characters
 */
size_t pad_char(char *str, char *ch)
{
	size_t pc = 0;

	if (str)
	{
		pc = strspn(str, ch);
		if (pc && !str[pc])
			pc--;
	}

	return (pc);
}
