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
 * Return: number of characters in line, -1 on read fail, -2 on malloc fail
 */
long int _getline(char **line, ssize_t *ln_sz, int fd)
{
	static long int byt_cnt, crnt_i;
	static char buff[BUFFER_SIZE];
	ssize_t i = 0, rd_sz = 0;

	if (byt_cnt <= 0)
	{
		crnt_i = 0;
		byt_cnt = 0;
		_memset(buff, '\0', BUFFER_SIZE);
		rd_sz = read(fd, buff, BUFFER_SIZE);
		if (rd_sz > 0)
			byt_cnt = rd_sz;
		else if (rd_sz == 0)
			exit(EXIT_SUCCESS);
		else
			return (rd_sz);
	}
	else
	{
		byt_cnt -= (crnt_i + 1);
		if (byt_cnt < 1)
		{
			*ln_sz = 0;
			return (0);
		}
	}

	i = find_line(buff, crnt_i);
	if (!(*line) || (*ln_sz < i))
		*line = mem_line(*line, i);
	if (!(*line))
		return (-2);

	*line = _strncpy(*line, &buff[crnt_i], i);
	(*line)[i] = '\0';
	*ln_sz = i;
	crnt_i += i;
	if (rd_sz == -1)
		return (-1);
	else if (rd_sz == 0)
		return (0);

	return (i);
}

/**
 * find_line - finds the size of a line from a buffer
 * @buff: a buffer containing characters
 * @crnt_i: index to begin searching for line.
 *
 * Return: Number of bytes in the line
 */
ssize_t find_line(char *buff, int crnt_i)
{
	ssize_t i = 0;

	while ((buff[crnt_i + i] != '\n') && ((crnt_i + i) < BUFFER_SIZE - 1))
		i++;

	if ((crnt_i + i) >= BUFFER_SIZE - 1)
		while (!buff[crnt_i + i])
			i--;

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
