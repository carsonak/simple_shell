#include "shell.h"

/**
 * isPath - searches directories in the PATH for the command
 * @cmd: address of file to be searched for
 *
 * Description: isPath() will search directories in the path environment
 * variable for the given file. If the the file is found it will modify the
 * given string to reflect the absolute pathname of the given file, else the
 * string is not changed.
 *
 * Return: 1 if file was found, 0 if not found, -1 on any failure
 */
int isPath(char **cmd)
{
	char *environ = NULL, *directory = NULL;
	int err = 0;

	if (!is_abs_path(*cmd))
		return (searchDIR(cmd, NULL));

	environ = _strdup(_getenv("PATH"));
	if (!environ)
		return (0);

	directory = _strtok(environ, ":");
	while (directory)
	{
		err = searchDIR(&directory, *cmd);
		if (err == -1)
		{
			free(environ);
			return (-1);
		}

		if (err == 1)
			break;

		directory = _strtok(NULL, ":");
	}

	if (directory)
		*cmd = make_path(directory, *cmd);
	else
		return (0);

	free(environ);
	if (errno || !(*cmd))
		return (-1);

	return (1);
}

/**
 * is_abs_path - check if first character/s of cmd are "/" or "./" or "../"
 * @cmd: the string to be checked
 *
 * Return: 0 if is absolute pathname, a +/- int if not
 */
int is_abs_path(char *cmd)
{
	int diff = 0;

	diff = _strncmp(cmd, "/", 1);
	if (diff)
		diff = _strncmp(cmd, "./", 2);

	if (diff)
		diff = _strncmp(cmd, "../", 3);

	return (diff);
}

/**
 * make_path - concatenates a directory string with a file string
 * @directory: the directory pathname
 * @file: the filename
 *
 * Return: pointer to the absolute pathname, NULL on failure
 */
char *make_path(char *directory, char *file)
{
	char *path = NULL;
	size_t p_len = _strlen(directory) + _strlen(file) + 2;

	if (!directory || !file)
		return (NULL);

	path = malloc(sizeof(*path) * p_len);
	_memset(path, '\0', p_len);
	if (path)
	{
		_strncpy(path, directory, _strlen(directory));
		_strncat(path, "/\0", 1);
		_strncat(path, file, _strlen(file));
	}
	else
		perror("make_path: Malloc fail");

	return (path);
}
