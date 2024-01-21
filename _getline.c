#include "shell.h"

/**
 * _getline - reads input from a file descriptor and breaks it into lines
 * @line: address of a buffer to store the line
 * @ln_sz: address of an int to store length of the line
 * @fd: file descriptor to read from
 *
 * Description: _getline will read a certain number of bytes from the file
 * descriptor storing them in a buffer. It will then parse the bytes into
 * lines which it returns one by one till there are no more bytes to be read.
 * EOF forces read to capture all that is in standard input and return. If
 * there were no bytes to be read, stdin is closed? making all consequent reads
 * to return 0.
 *
 * Return: number of characters in line, 0 on EOF, -1 on failure
 */
ssize_t _getline(char **line, size_t *ln_sz, int fd)
{
	static ssize_t byt_cnt, crnt_i;
	static char buff[BUFFER_SIZE];
	ssize_t len = 0;

	if (!line)
		return (-1);

	if (byt_cnt < 1)
	{
		crnt_i = 0;
		_memset(buff, '\0', BUFFER_SIZE);
		byt_cnt = read(fd, buff, BUFFER_SIZE - 2);
		if (byt_cnt < 1)
			return (byt_cnt);
	}

	free(*line);
	len = find_line(buff, crnt_i, byt_cnt);
	*line = line_alloc(len, &buff[crnt_i]);
	if (!(*line))
		return (-1);

	*ln_sz = len;
	crnt_i += len;
	byt_cnt -= len;
	if ((*line)[len - 1] == '\n')
		(*line)[len - 1] = '\0';

	return (len);
}

/**
 * find_line - finds the next "\n" character in a buffer
 * @buff: a buffer of characters
 * @crnt_i: starting index
 * @byt_cnt: maximum bytes that can be read
 *
 * Return: total bytes read
 */
ssize_t find_line(char *buff, ssize_t crnt_i, ssize_t byt_cnt)
{
	ssize_t i = 0;

	while ((buff[crnt_i + i] != '\n') && i < (byt_cnt - 1))
		i++;

	return (i + 1);
}

/**
 * line_alloc - read len bytes from a buffer and stores them in a string
 * @len: number of bytes to be read
 * @buff: buffer to read from
 *
 * Return: pointer to the string, NULL on failure
 */
char *line_alloc(ssize_t len, char *buff)
{
	char *line = malloc((sizeof(*line) * len) + 1);

	if (!line)
		return (NULL);

	_strncpy(line, buff, len);
	line[len] = '\0';
	return (line);
}
