#ifndef SIMPLE_SHELL_STRINGS_H
#define SIMPLE_SHELL_STRINGS_H

#include <inttypes.h> /* intmax_t */

#include "alloc.h"

/**
 * struct string - a mutable string data type.
 * @size: size of the string in bytes, should not be less than 0.
 * @i: cursor for the string.
 * @s: pointer to the beginning of the string.
 */
struct string
{
	intmax_t size;
	intmax_t i;
	char *s;
};

/**
 * struct view_string - a read only string data type.
 * @size: size of the string in bytes, should not be less than 0.
 * @i: cursor for the string.
 * @s: pointer to the beginning of the string.
 */
struct view_string
{
	intmax_t size;
	intmax_t i;
	const char *s;
};

typedef struct string string;
typedef struct view_string view_string;

void string_delete(void *const data);
void *string_dup(const void *const data);
void *cstr_to_string(const char *const cstr);
void *string_to_cstr(const void *const data);

char string_readc(view_string *const s);
char string_readp(view_string *const s);
char string_peekc(view_string const *const s);
char string_peekp(view_string const *const s);
char string_peekn(view_string const *const s);

view_string _strstr(const char *const haystack, const char *const needle);
view_string _strtok(
	view_string *const str, const char *const delimeters);
intmax_t _strlen(const char *const s);
short int _strncmp(const char *s1, const char *s2, intmax_t len);

char *_memset(char *const mem, const char byte, intmax_t size);
char *_memcpy(char *const dest, const char *const src, const intmax_t n);
char *_strncat(char *dest, const char *const s, intmax_t n);
char *_strdup(const char *const str, intmax_t size);
char *strjoin(intmax_t n, ...);
char *strconcat(const char *const s1, const char *const s2);

#endif /* SIMPLE_SHELL_STRINGS_H */
