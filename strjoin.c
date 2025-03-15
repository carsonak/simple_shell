#include <stdarg.h>

#include "strings.h"

/**
 * strjoin - concatenates a variable number of strings.
 * @n: the number of strings to join.
 *
 * Return: pointer to the concatenated string, NULL on failure.
 */
char *strjoin(intmax_t n, ...)
{
	char *long_str = NULL;
	intmax_t long_str_len = 0;
	va_list strings;

	va_start(strings, n);
	while (n > 0)
	{
		char *str_n = va_arg(strings, char *);
		intmax_t str_n_len = _strlen(str_n);

		long_str_len += str_n_len;
		long_str = _realloc_free_on_fail(
			long_str, long_str_len + 1, long_str_len - str_n_len);
		if (!long_str)
			break;

		long_str = _strncat(long_str, str_n, str_n_len);
		--n;
	}

	va_end(strings);
	return (long_str);
}
