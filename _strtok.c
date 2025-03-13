#include <assert.h>

#include "strings.h"

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
