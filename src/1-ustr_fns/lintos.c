#include "u_string.h"

/**
 * lintos - converts a long int to a string
 * @num: a long int
 *
 * Return: pointer to the string, NULL on failure
 */
char *lintos(ssize_t num)
{
	ssize_t div = 1, idx = 0, val = num, i = 0;
	char *num_str = NULL;

	if (num < 0)
	{
		num = -val;
		idx++;
	}

	while (val / div >= 10)
	{
		div *= 10;
		idx++;
	}

	num_str = malloc(sizeof(*num_str) * (idx + 2));
	if (num_str)
	{
		if (num < 0)
		{
			num_str[0] = '-';
			i++;
		}

		while (i <= idx && div)
		{
			num_str[i] = ((val / div) + '0');
			val %= div;
			div /= 10;
			i++;
		}

		num_str[i] = '\0';
	}

	return (num_str);
}
