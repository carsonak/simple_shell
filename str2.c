#include "strings.h"
#include "alloc.h"

/**
 * _strdup - duplicate a string in memory.
 * @str: the string to duplicate.
 * @size: size of the string.
 *
 * Return: pointer to the duplicated string, NULL on error.
 */
char *_strdup(const char *const str, intmax_t size)
{
	char *new_str = NULL;

	if (!str || size < 1)
		return (NULL);

	new_str = _memncpy(_malloc(sizeof(*str) * size + 1), str, size);
	new_str[size] = '\0';
	return (new_str);
}

/**
 * _strstr - searches for a substring in a given string.
 * @haystack: the string.
 * @needle: the substring to search for.
 *
 * Return: a `view_string` pointing to the start of substring if found,
 * otherwise the `view_string` points to NULL.
 */
view_string _strstr(const char *const haystack, const char *const needle)
{
	view_string s = {0};
	intmax_t n_i = 0, h_i = 0;

	if (!haystack || !needle)
		return (s);

	for (h_i = 0; haystack[h_i]; ++h_i)
	{
		for (n_i = 0; needle[n_i] && needle[n_i] == haystack[h_i]; n_i++)
			;

		if (needle[n_i] == '\0')
		{
			s.s = &haystack[h_i];
			s.size = n_i;
			return (s);
		}
	}

	return (s);
}
