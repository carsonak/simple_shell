#include "shell.h"

/**
 * _strtok - breaks a string into sections
 * @str: the string
 * @delim: the delimeter
 *
 * Return: pointer to the tokenised string
 */
char *_strtok(char *str, char *delim)
{
	static char *tok_ptr;
	static long int str_len;
	char *tok_b;
	unsigned int i = 0;

	if (str)
	{
		tok_ptr = str;
		str_len = _strlen(tok_ptr);
	}

	if (str_len <= 0)
		return (NULL);

	i = _strspn(tok_ptr, delim);
	str_len -= i;
	tok_ptr += i;
	tok_b = tok_ptr;
	if (str_len <= 0)
		return (NULL);

	while (str_len > 0)
	{
		for (i = 0; delim[i]; i++)
		{
			if (*tok_ptr == delim[i])
			{
				*tok_ptr = '\0';
				break;
			}
		}

		tok_ptr++;
		str_len--;
		if (delim[i])
			break;
	}

	return (tok_b);
}
