#include "shell.h"

/**
 * _getenv - gets a particular environment value
 * @name: the value
 *
 * Return: pointer to the value, NULL on failure
 */
char *_getenv(char *name)
{
	char **ep = NULL;
	unsigned long int len = 0;

	if (__environ == NULL || name[0] == '\0')
		return (NULL);

	len = _strlen(name);
	for (ep = __environ; *ep != NULL; ++ep)
		if (name[0] == (*ep)[0] && !(_strncmp(name, *ep, len)) && (*ep)[len] == '=')
			return (*ep + len + 1);

	return (NULL);
}
