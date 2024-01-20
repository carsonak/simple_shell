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
int searchDIR(char **dirPath, char *file)
{
	DIR *folder = NULL;
	struct dirent *entry = NULL;

	if (!file)
	{
		if ((*dirPath)[0] == '/')
			return (1);
		else
			return (abs_search(dirPath));
	}

	folder = opendir(*dirPath);
	if (folder)
	{
		errno = 0;
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

		if (!entry && errno)
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
 * @path: pointer to the absolute pathname
 *
 * Return: 1 if file was found, 0 if not found, -1 on error.
 */
int abs_search(char **path)
{
	char *p_cpy = NULL, *cwd = NULL;
	size_t len = 0;
	int err = 0;

	cwd = _getenv("PWD");
	cwd = _strdup(cwd);
	if (!cwd || path)
		return (-1);

	p_cpy = append_dir(*path, cwd);
	if (!p_cpy)
		return (-1);

	len = _strlen(p_cpy);
	while (p_cpy[len] != '/')
		len--;

	if (len)
	{
		p_cpy[len] = '\0';
		err = searchDIR(&p_cpy, &p_cpy[len + 1]);
		p_cpy[len] = '/';
	}

	free(*path);
	*path = p_cpy;
	return (err);
}

/**
 * append_dir - append appropriate directory to a relative path
 * @path: the relative path
 * @cwd: current working directory
 *
 * Return: the full path on success, NULL on failure
 */
char *append_dir(char *path, char *cwd)
{
	size_t c_i = 0, spn = 0;
	char *p_cpy = NULL;

	spn = _strspn(path, "./");
	if (spn < 2)
		spn = 0;
	else if (spn > 3)
		spn = 3;

	p_cpy = _strdup(path + spn);
	if (!cwd || !p_cpy || path[0] == '/')
		return (NULL);

	c_i = (_strlen(cwd) - 1);
	if (cwd[c_i] == '/')
		cwd[c_i] = '\0';

	if (spn && (pad_char(path, ".") > 1))
	{
		while (c_i > 0 && cwd[c_i] != '/')
			c_i--;

		if (c_i)
			cwd[c_i] = '\0';
	}

	return (stringscat(2, cwd, p_cpy));
}
