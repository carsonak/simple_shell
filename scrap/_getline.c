#include "shell.h"

/**
 * _getline - reads a line (ends with a '\n') from a file descriptor.
 * @line: adress of a buffer to store the line
 * @n_c: size of the buffer?
 * @fd: file descriptor to be read from
 *
 * Return: number of characters read, -1 on failure
 */
int _getline(char **line, int *n_c, int fd)
{
	char buffer[BUFFER_SIZE];
	ssize_t err = 0;
	unsigned long int i = 0;

	_memset(buffer, '\0', BUFFER_SIZE);

	err = read(fd, buffer, BUFFER_SIZE);
	if (err == -1)
		return (-1);

	for (i = 0; (i < BUFFER_SIZE - 1) && (buffer[i] != '\n'); i++)
		;

	i += 1;
	if (!(*line) || sizeof(*line) < i)
	{
		*line = malloc(i);
		if (!(*line))
			return (-1);
	}

	*line = _strncpy(*line, buffer, i);
	*n_c = i;

	return (i);
}
