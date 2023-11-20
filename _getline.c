#include "shell.h"

/**
 * _getline - reads input from a file descriptor and breaks it into lines
 * @line: address of a buffer to store the line
 * @ln_sz: address of an int to store length of the line
 * @fd: file descriptor to read from
 *
 * Description: This function does not work the same as the std getline
 * function.
 *
 * Return: number of characters in line, -1 on fail, -2 on EOF
 */
long int _getline(char **line, ssize_t *ln_sz, int fd)
{
	static ssize_t byt_cnt, crnt_i;
	static char buff[BUFFER_SIZE];
	ssize_t len = 0;

	if (byt_cnt <= 0)
	{
		crnt_i = 0;
		byt_cnt = byteRd(fd, &buff[0]);
		if (byt_cnt == -1)
			return (-1);
		else if (byt_cnt == 0)
			return (-2);
	}
	else
	{
		byt_cnt -= (crnt_i);
		if (byt_cnt < 1)
			return (0);
	}

	len = find_line(buff, crnt_i, byt_cnt);
	if (!(*line) || (*ln_sz < len))
		*line = mem_line(*line, len);

	if (!(*line))
	{
		*ln_sz = 0;
		byt_cnt = 0;
		return (-1);
	}

	*line = _strncpy(*line, &buff[crnt_i], len);
	(*line)[len] = '\0';
	*ln_sz = len;
	crnt_i += len;
	return (len);
}

/**
 * byteRd - reads from a file descriptor and returns number of bytes read
 * @fd: the file descriptor
 * @buff: address of a buffer to read from
 *
 * Return: total bytes read if successful, -1 on failure
 */
ssize_t byteRd(int fd, char *buff)
{
	ssize_t rd_sz = 0, i = BUFFER_SIZE - 1;

	_memset(buff, '\0', BUFFER_SIZE);
	rd_sz = read(fd, buff, BUFFER_SIZE);
	if (!rd_sz)
	{
		while (!buff[i] && i >= 0)
			i -= 1;

		rd_sz = i + 1;
	}

	return (rd_sz);
}

/**
 * find_line - breaks characters in a buffer into lines.
 * @buff: a buffer containing characters
 * @crnt_i: starting index
 * @byt_cnt: position of the last recorded byte in the buffer + 1.
 *
 * Return: total bytes in the line
 */
ssize_t find_line(char *buff, ssize_t crnt_i, ssize_t byt_cnt)
{
	ssize_t i = 0;

	while ((buff[crnt_i + i] != '\n') && (crnt_i + i) < (byt_cnt - 1))
		i++;

	return (i + 1);
}

/**
 * mem_line - allocates memory for a string
 * @line: a buffer
 * @sz: size in bytes to be allocated
 *
 * Return: pointer to allocated memory, NULL on failure
 */
char *mem_line(char *line, int sz)
{
	if (line)
		free(line);

	line = malloc((sizeof(*line) * sz) + 1);
	return (line);
}
