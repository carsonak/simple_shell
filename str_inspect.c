#include <assert.h>

#include "strings.h"

/**
 * _strlen - counts number of characters in a string upto
 * the terminating null byte.
 * @s: pointer to the string.
 *
 * Return: length of the string.
 */
intmax_t _strlen(const char *const s)
{
	intmax_t i;

	if (!s)
		return (0);

	for (i = 0; s[i]; ++i)
		;

	return (i);
}

/**
 * _strncmp - compares the first `len` characters of two strings.
 * @s1: pointer to the first string.
 * @s2: pointer to the second string.
 * @len: number of characters to compare, cannot be less than 0.
 *
 * Return: 0 if strings are equal,
 * the difference of the first differing characters otherwise.
 */
short int _strncmp(const char *s1, const char *s2, intmax_t len)
{
	if (!s1 || !s2 || len < 1)
		return (0);

	while (len > 0 && !(*s1 - *s2))
	{
		++s1;
		++s2;
		--len;
	}

	return (*s1 - *s2);
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

#define _STRTOK_FILTER_SIZE 256U

/**
 * initialise_filter - initialise a character filter for a given set of
 * characters.
 * @filter: pointer to the filter to initialise.
 * @to_filter: pointer to a string of characters to initialise with.
 */
static void initialise_filter(
	unsigned char *const filter, const char *to_filter)
{
	assert(filter && to_filter);
	_memset((char *)filter, 0, _STRTOK_FILTER_SIZE);
	while (*to_filter)
	{
		filter[(unsigned char)*to_filter] = 1;
		++to_filter;
	}
}

/**
 * _strtok - returns substrings of `str` that do not contain any character
 * in `delims`.
 * @str: pointer to the string to tokenize.
 * @delims: the delimiting characters.
 *
 * The most recent value of `delims` will be used if NULL is passed in its
 * place from the second call onwards.
 * If there are no more substrings, a token pointing to NULL will be returned.
 * It is an error if `str` is not properly initialised or no delimeters were
 * initially provided.
 *
 * Return: the initial part of the string ending with one of the characters in
 * `delims`, NULL token on error.
 */
view_string _strtok(
	view_string *const str, const char *const delims)
{
	static unsigned char filter[_STRTOK_FILTER_SIZE];
	char c = 0;
	view_string token = {0};

	if (!str)
		return (token);

	if (delims)
		initialise_filter(filter, delims);

	c = string_readc(str);
	while (c > -1 && filter[(unsigned char)c] == 1)
		c = string_readc(str);

	if (c < 0)
		return (token);

	token.s = &str->s[str->i - 1];
	while (c > -1 && filter[(unsigned char)c] == 0)
	{
		++token.size;
		c = string_readc(str);
	}

	if (c > -1)
		string_readp(str);

	return (token);
}
