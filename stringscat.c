#include "shell.h"

/**
 * stringscat - concatenates a variable number of strings
 * @items: the number of strings to be concatenated
 *
 * Return: pointer to the resultant string, NULL on failure
 */
char *stringscat(size_t items, ...)
{
	va_list ptr;
	char *s1 = NULL, *s2 = NULL;
	size_t i = items;

	if (!items)
		return (malloc(sizeof(*s1)));

	va_start(ptr, items);
	while (i)
	{
		s2 = s1;
		s1 = str_concat(s2, va_arg(ptr, char *));
		if (s2)
			free(s2);

		if (!s1)
			break;

		i--;
	}

	va_end(ptr);
	return (s1);
}

/**
 * str_concat - concatinates 2 strings and stores them in a new array.
 * @s1: 1st string
 * @s2: 2nd string
 *
 * Return: a pointer to the new string, NULL if it fails
 */
char *str_concat(char *s1, char *s2)
{
	char *bGstr = "";
	size_t i = 0, j = 0;

	if (!s1 && !s2)
		return (malloc(sizeof(*bGstr)));
	else if (!s1 && s2)
		bGstr = malloc((sizeof(*bGstr) * _strlen(s2)) + 1);
	else if (s1 && !s2)
		bGstr = malloc((sizeof(*bGstr) * _strlen(s1)) + 1);
	else
		bGstr = malloc((sizeof(*bGstr) * (_strlen(s1) + _strlen(s2))) + 1);

	if (!bGstr)
		return (NULL);

	if (s1)
		for (i = 0; s1[i]; i++, j++)
			bGstr[j] = s1[i];

	if (s2)
		for (i = 0; s2[i]; i++, j++)
			bGstr[j] = s2[i];

	if (j)
		bGstr[j] = '\0';

	return (bGstr);
}
