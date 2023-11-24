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

	if (!file && (*dirPath)[0] != '/')
		return (abs_search(dirPath));
	else if (!file && (*dirPath)[0] == '/')
		return (1);

	folder = opendir(*dirPath);
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
int abs_search(char **path)
{
	unsigned long int len = 0;
	int err = 0;
	char *p_cpy = NULL;

	if (!rel_path(path))
		return (-1);

	len = _strlen(*path);
	p_cpy = _strdup(*path);
	if (!p_cpy)
		return (-1);

	while (p_cpy[len] != '/')
		len--;

	if (len)
	{
		p_cpy[len] = '\0';
		err = searchDIR(&p_cpy, &p_cpy[len + 1]);
	}

	free(p_cpy);
	return (err);
}

/**
 * rel_path - appends the appropriate working directory to a relative path
 * @path: the path
 *
 * Return: 1 on success, 0 on failure
 */
int rel_path(char **path)
{
	size_t len = 0, spn = 0, i = 0;
	char *p_cpy = NULL, *cwd = NULL;
	int j = 0;

	cwd = _getenv("PWD");
	cwd = _strdup(cwd);
	p_cpy = _strdup(*path);
	if (!cwd || !p_cpy)
		return (0);

	spn = _strspn(p_cpy, "./");
	for (j = 0; p_cpy[j] && i < spn && spn > 1; j++)
	{
		if (!_strncmp("../", &p_cpy[j], 3) || !_strncmp("./", &p_cpy[j], 2))
		{
			i += _strcspn(&p_cpy[j], "/") + 1;
			for (len = (_strlen(cwd) - 1); len && (cwd[len] != '/'); len--)
				;

			if (cwd[len] == '/' && len && !_strncmp("../", &p_cpy[j], 3))
				cwd[len] = '\0';

			p_cpy = trim_str(p_cpy, (j + _strcspn(&p_cpy[j], "/")));
			if (!p_cpy)
				return (0);

			j = -1;
		}
	}

	if (spn > 1)
	{
		free(*path);
		*path = str_concat(cwd, p_cpy);
	}

	free(cwd);
	free(p_cpy);
	return (1);
}

/**
 * trim_str - trims a string?
 * @str: a malloced string
 * @cut: index to trim at
 *
 * Return: pointer to string, NULL on failure
 */
char *trim_str(char *str, int cut)
{
	char *nwStr = NULL;

	nwStr = _strdup(&str[cut]);
	free(str);

	return (nwStr);
}
