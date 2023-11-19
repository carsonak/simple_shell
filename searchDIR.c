#include "shell.h"

/**
 * searchDIR - searches for a file within a directory
 * @dirPath: the directory
 * @file: the file
 *
 * Description: This function can be used to search directories for the
 * given files. If file is set to NULL, dirPath is treated as an absolute
 * pathname.
 *
 * Return: 1 if file was found, 0 if not found, -1 on error.
 */
int searchDIR(char *dirPath, char *file)
{
	DIR *folder = NULL;
	struct dirent *entry = NULL;

	if (!file)
		return (abs_search(dirPath));

	folder = opendir(dirPath);
	if (folder)
	{
		entry = readdir(folder);
		while (entry)
		{
			if (!(_strncmp(entry->d_name, file, (_strlen(file) + 1))))
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

/**
 * abs_search - checks if an absolute pathname exists
 * @path: the absolute pathname
 *
 * Return: 1 if file was found, 0 if not found, -1 on error.
 */
int abs_search(char *path)
{
	unsigned long int len = 0;
	int err = 0;
	char *path_cpy = NULL;

	len = _strlen(path);
	path_cpy = _strdup(path);
	if (!path_cpy)
		return (-1);

	while (path_cpy[len] != '/')
		len--;

	if (len)
	{
		path_cpy[len] = '\0';
		err = searchDIR(path_cpy, &path_cpy[len + 1]);
	}

	free(path_cpy);
	return (err);
}
