#include "string.h"

/**
 * string_dup - copies a `view_string` data type into a `string` data type.
 * @data: the string to duplicate.
 *
 * Return: pointer to the duplicated string.
 */
void *string_dup(const void *const data)
{
	const view_string *const src = data;
	string *dup;

	if (!src)
		return (NULL);

	dup = _calloc(1, sizeof(*dup));
	if (!dup)
		return (NULL);

	dup->size = src->size;
	dup->s = _strdup(src->s, src->size);
	if (!dup->s)
		return (_free(dup));

	dup->s[dup->size] = '\0';
	return (dup);
}

/**
 * string_to_cstr - extract a C string from a `string` type.
 * @data: pointer to the `string` type.
 *
 * Return: pointer to the duplicated string, NULL on failure.
 */
void *string_to_cstr(void *data)
{
	string *s = data;

	if (!s || !s->s || s->size < 1)
		return (NULL);

	return (_strdup(s->s, s->size));
}

/**
 * cstr_to_string - instantiate a string type with a C string.
 * @cstr: a null terminated char array.
 *
 * Return: pointer to the created string type, NULL on error.
 */
void *cstr_to_string(const char *const cstr)
{
	view_string str = {0};

	if (!cstr)
		return (NULL);

	str.s = cstr;
	str.size = _strlen(cstr) + 1;
	return (string_dup(&str));
}

/**
 * string_delete - frees a `string` data type from memory.
 * @data: pointer to the string to free.
 */
void string_delete(void *const data)
{
	string *s = data;

	if (s)
	{
		s->s = (const char *)_free(s->s);
		s->size = 0;
	}

	_free(s);
}
