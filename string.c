#include "string.h"
#include "alloc.h"

/**
 * string_dup - duplicates a `string` data type.
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
	dup->s = _memcpy(_malloc(sizeof(*dup->s) * src->size + 1), src->s, src->size);
	if (!dup->s)
		return (_free(dup));

	dup->s[dup->size] = '\0';
	return (dup);
}

/**
 * string_delete - frees a `string` data type from memory.
 * @data: pointer to the string to free.
 */
void string_delete(void *const data)
{
	view_string *s = data;

	if (s)
	{
		s->s = (const char *)_free(s->s);
		s->size = 0;
	}

	_free(s);
}
