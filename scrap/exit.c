#include "scrap.h"
/**
 **_strncpy - copies a string
 *@d: the destination string to be copied to
 *@src: the source string
 *@m: the amount of characters to be copied
 *Return: the concatenated string
 */
char *_strncpy(char *d, char *src, int m)
{
	int i, j;
	char *s = d;

	i = 0;
	while (src[i] != '\0' && i < m - 1)
	{
		d[i] = src[i];i++;
	}
	if (i < m)
	{
		j = i;
		while (j < m)
		{
			d[j] = '\0';j++;
			
		}
	}
	return (s);
}

/**
 **_strncat - concatenates two strings
 *@d: the first string
 *@src: the second string
 *@m: the amount of bytes to be maximally used
 *Return: the concatenated string
 */
char *_strncat(char *d, char *src, int m)
{
	int i, j;
	char *s = d;

	i = 0;
	j = 0;
	while (src[j] != '\0' && j < m)
	{
		d[i] = src[j];
		i++;
        
	}
	while (d[i] != '\0')
		i++;
	
	if (j < m)
		d[i] = '\0';
	return (s);
}

/**
 **_strchr - locates a character in a string
 *@s: the string to be parsed
 *@c: the character to look for
 *Return: (s) a pointer to the memory area s
 */
char *_strchr(char *s, char c)
{
	do {
		if (*s == c)
			return (s);
	} while (*s++ != '\0');

	return (NULL);
}
