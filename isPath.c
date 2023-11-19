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
		return (searchDIR(*cmd, NULL));

	environ = _strdup(_getenv("PATH"));
	if (!environ)
		return (-1);

	directory = _strtok(environ, ":");
	while (directory)
	{
		err = searchDIR(directory, *cmd);
		if (err == -1)
		{
			free(environ);
			return (-1);
		}
		else if (err == 1)
			break;

		directory = _strtok(NULL, ":");
	}

	if (directory)
		*cmd = make_path(directory, *cmd);
	else
	{
		free(environ);
		return (0);
	}

	free(environ);
	if (errno || !(*cmd))
		return (-1);

	return (1);
}

/**
 * is_abs_path - checks if the first character/s of cmd is "/" or "./"
 * @cmd: the string to be checked
 *
 * Return: 0 if is absolute pathname, a +/- int if not
 */
int is_abs_path(char *cmd)
{
	int err = 0;

	err = _strncmp(cmd, "/", 1);
	if (err)
		err = _strncmp(cmd, "./", 2);

	return (err);
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

	path = malloc(sizeof(*directory) * (_strlen(directory) + _strlen(file)) + 2);
	if (path)
	{
		path = _strncpy(path, directory, (_strlen(directory) + 1));
		path = _strncat(path, "/", 2);
		path = _strncat(path, file, (_strlen(file) + 1));
		free(file);
	}
	else
		return (NULL);

	return (path);
}
