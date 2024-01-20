#ifndef _U_STRING_H_
#define _U_STRING_H_

#include <stdio.h> /*perror()*/
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h> /*variadic functions*/

int _strncmp(char *s1, char *s2, size_t n);
size_t pad_char(char *str, char *ch);
size_t _strlen(char *s);
size_t _strspn(char *s, char *accept);
size_t _strcspn(char *s, char *reject);
char *stringscat(size_t items, ...);
char *str_concat(char *s1, char *s2);
char *_strncat(char *dest, char *src, size_t n);
char *_strncpy(char *dest, char *src, size_t n);
char *_strcpy(char *dest, char *src);
char *_strtok(char *str, char *delim);
char *_strdup(char *str);
char *lintos(ssize_t num);

#endif /*_USTRING_H_*/
