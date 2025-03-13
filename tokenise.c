#include "simple_shell.h"

#include <assert.h>

static view_string get_double_quoted_str(view_string *str)
{
	view_string quote = {0};
	unsigned char is_escaped = 0;
	char c = 0;

	assert(str);
	assert(str->s && str->i > -1 && str->size > 0);
	quote.s = &str->s[str->i];
	c = string_readc(str);
	assert(c == '"');
	c = string_readc(str);
	for (quote.size = 1; c > -1; ++quote.size, c = string_readc(str))
	{
		if (is_escaped == 1)
		{
			is_escaped = 0;
			continue;
		}

		if (c == '\\')
			is_escaped = 1;

		if (c == '"')
			break;
	}

	if (c != '"')
	{
		quote.size = 0;
		quote.s = NULL;
		return (quote);
	}

	++quote.size;
	return (quote);
}

static view_string get_single_quoted_str(view_string *str)
{
	view_string quote = {0};
	char c = 0;

	assert(str);
	assert(str->s && str->i > -1 && str->size > 0);
	quote.s = &str->s[str->i];
	c = string_readc(str);
	assert(c == '\'');
	c = string_readc(str);
	for (quote.size = 0; c > -1 && c != '\'';
		 ++quote.size, c = string_readc(str))
		;

	if (c != '\'')
	{
		quote.size = 0;
		quote.s = NULL;
		return (quote);
	}

	++quote.size;
	return (quote);
}

queue *tokenise(const char *const command_str, intmax_t size)
{
	view_string str = {0}, tok = {0};
	queue *tokens = queue_new();

	assert(command_str && size > 0);
	str.s = command_str;
	str.size = size;
	if (!tokens)
		return (NULL);

	tok = _strtok(&str, " \t");
	while (tok.s)
	{
		if (*tok.s == '"')
		{
			str.i -= tok.size;
			tok = get_double_quoted_str(&str);
		}

		if (*tok.s == '\'')
		{
			str.i -= tok.size;
			tok = get_single_quoted_str(&str);
		}

		if (!tok.s)
			return (queue_delete(tokens, string_delete));

		if (!enqueue(tokens, &tok, string_dup))
			return (queue_delete(tokens, string_delete));

		tok = _strtok(&str, NULL);
	}

	return (tokens);
}
