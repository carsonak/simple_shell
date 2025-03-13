#include "strings.h"

/**
 * strconcat - concatenates two c-strings in a new memory block.
 * @s1: first string.
 * @s2: second string.
 *
 * Return: pointer to the concatenated string.
 */
char *strconcat(const char *const s1, const char *const s2)
{
	intmax_t s1_len = _strlen(s1);
	intmax_t s2_len = _strlen(s2);
	intmax_t s3_len = s1_len + s2_len + 1;
	char *s3 = NULL;

	if (s1_len < 1 && s2_len < 1)
		return (NULL);

	s3 = _malloc(sizeof(*s3) * s3_len);
	if (!s3)
		return (NULL);

	if (s1_len > 0)
		_memcpy(s3, s1, s1_len + 1);

	if (s2_len > 0)
		_memcpy(s3, s2, s2_len + 1);

	return (s3);
}
