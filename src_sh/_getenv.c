#include "shell.h"

/**
 * _getenv - gets a particular environment value
 * @name: the value
 *
 * Return: pointer to the value, NULL on failure
 */
char *_getenv(char *name)
{
	char **enV = NULL;
	size_t len = 0;

	if (__environ == NULL || name[0] == '\0' || !name)
		return (NULL);

	len = _strlen(name);
	for (enV = __environ; *enV; ++enV)
		if (name[0] == (*enV)[0] && !(_strncmp(name, *enV, len)) &&
			(*enV)[len] == '=')
			return (*enV + len + 1);

	return (NULL);
}
