#include <limits.h>

#include "string.h"

/**
 * string_readc - returns current character and advances the string cursor.
 * @s: the string.
 *
 * An error is returned if: the cursor position < 0 or cursor position > size
 * or size <= 0.
 * If the cursor is at the end, CHAR_MIN will be returned.
 *
 * Return: the current character, -1 on error.
 */
char string_readc(view_string *const s)
{
	if (!s || s->i < 0 || s->size <= 0 || s->i > s->size)
		return (-1);

	if (s->i == s->size)
		return (CHAR_MIN);

	return (s->s[s->i++]);
}

/**
 * string_readp - returns previous character and retreats the string cursor.
 * @s: the string.
 *
 * An error is returned if: the cursor position < 0 or cursor position >= size
 * or size < 0.
 * If there are no more characters before the cursor,
 * CHAR_MIN will be returned.
 *
 * Return: the previous character, -1 on error.
 */
char string_readp(view_string *const s)
{
	if (!s || s->i < 0 || s->size <= 0 || s->i > s->size)
		return (-1);

	if (s->i == 0)
		return (CHAR_MIN);

	return (s->s[--s->i]);
}

/**
 * string_peekc - returns character at the string cursor.
 * @s: the string.
 *
 * An error is returned if: the cursor position < 0 or cursor position > size
 * or size <= 0.
 * If the cursor is at the end, CHAR_MIN will be returned.
 *
 * Return: the current character, -1 on error.
 */
char string_peekc(view_string const *const s)
{
	if (!s || s->i < 0 || s->size <= 0 || s->i > s->size)
		return (-1);

	if (s->i == s->size)
		return (CHAR_MIN);

	return (s->s[s->i]);
}

/**
 * string_peekp - returns character before the string cursor.
 * @s: the string.
 *
 * An error is returned if: the cursor position < 0 or cursor position >= size
 * or size < 0.
 * If there are no more characters before the cursor,
 * CHAR_MIN will be returned.
 *
 * Return: the previous character, -1 on error.
 */
char string_peekp(view_string const *const s)
{
	if (!s || s->i < 0 || s->size <= 0 || s->i > s->size)
		return (-1);

	if (s->i == 0)
		return (CHAR_MIN);

	return (s->s[s->i - 1]);
}

/**
 * string_peekn - returns character after the string cursor.
 * @s: the string.
 *
 * An error is returned if: the cursor position < 0 or cursor position >= size
 * or size < 0.
 * If the cursor is at the end, CHAR_MIN will be returned.
 *
 * Return: the next character, -1 on error.
 */
char string_peekn(view_string const *const s)
{
	if (!s || s->i < 0 || s->size <= 0 || s->i > s->size)
		return (-1);

	if (s->i == s->size)
		return (CHAR_MIN);

	return (s->s[s->i + 1]);
}
