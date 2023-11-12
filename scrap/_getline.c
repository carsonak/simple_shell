#include <scrap.h>

/**
 * _getline - reads a line (ends with a '\n') from a file descriptor.
 * @lineptr: adress of a buffer to store the line
 * @n_c: size of the buffer?
 * @fd: file descriptor to be read from
 *
 * Return: number of characters read, -1 on failure
 */
int _getline(char **lineptr, int *n_c, int fd)
{
	char buffer[BUFFER_SIZE], *lBuf;
	ssize_t err = 0, i = 0;

	_memset(buffer, '\0', BUFFER_SIZE);

	err = read(fd, buffer, BUFFER_SIZE);
	if (err == -1)
		return (-1);

	for (i = 0; (i < BUFFER_SIZE) && (buffer[i] != '\n'); i++)
		;

	if (!(*lineptr) || sizeof(*lineptr) < i)
	{
		*lineptr = malloc(i);
		if (!(*lineptr))
			return (-1);
	}

	*lineptr = _strncpy(*lineptr, buffer, i);
	*n_c = i;

	return (i);
}
