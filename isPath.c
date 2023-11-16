#include "shell.h"

/**
 * isPath - searches the PATH environment variable for the command
 * @cmd: command to be searched for
 *
 * Return: absolute pathname of the command, NULL on failure
 */
char *isPath(char *cmd)
{
	char *environ = NULL, *directory = NULL, *path = NULL;
	int err = 0;

	environ = _getenv("PATH");
	if (environ)
	{
		environ = _strdup(environ);
		if (!environ)
		{
			perror("Failed to duplicate PATHs");
			return (path);
		}

		directory = _strtok(environ, ":");
		while (directory)
		{
			err = searchDIR(directory, cmd);
			if (err == -1)
				perror("searchDIR failure");
			else if (err == 1)
				break;

			directory = _strtok(NULL, ":");
		}

		if (directory)
		{
			path = malloc(sizeof(*directory) * (_strlen(directory) + _strlen(cmd)) + 2);
			if (path)
			{
				path = _strncpy(path, directory, (_strlen(directory) + 1));
				path = _strncat(path, "/", 2);
				path = _strncat(path, cmd, (_strlen(cmd) + 1));
			}
			else
				return (path);
		}
		free(environ);
	}
	return (path);
}

/**
 * searchDIR - searches a directory for a file
 * @directory: the directory
 * @file: the file
 *
 * Return: 1 if file was found, 0 if not found, -1 on error.
 */
int searchDIR(char *directory, char *file)
{
	DIR *folder = NULL;
	struct dirent *entry = NULL;

	errno = 0;
	folder = opendir(directory);
	if (folder)
	{
		entry = readdir(folder);
		while (entry)
		{
			if (!(_strncmp(entry->d_name, file, _strlen(file))))
			{
				closedir(folder);
				return (1);
			}

			entry = readdir(folder);
		}

		if ((entry == NULL) && (errno != 0))
		{
			closedir(folder);
			return (-1);
		}
	}
	else
		return (-1);

	closedir(folder);
	return (0);
}
