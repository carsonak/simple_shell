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

	if (byt_cnt < 1)
	{
		crnt_i = 0;
		_memset(buff, '\0', BUFFER_SIZE);
		byt_cnt = read(fd, buff, BUFFER_SIZE);
		if (byt_cnt == -1)
			return (-1);
		else if (!byt_cnt)
		{
			is_EOF(1);
			*ln_sz = len;
			return (0);
		}
	}

	len = find_line(buff, crnt_i, byt_cnt);
	*line = line_alloc(len, *line, &buff[0]);
	if (!(*line))
		return (-1);

	*ln_sz = len;
	crnt_i += len;
	if (byt_cnt <= crnt_i)
	{
		byt_cnt = 0;
		return (0);
	}

	return (len);
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
 * line_alloc - concatenates strings from _getline
 * @len: number of bytes to read from the input buffer
 * @line: line without a '\n'
 * @buff: input buffer from _getline
 *
 * Return: pointer to the concatenated string, NULL on failure
 */
char *line_alloc(ssize_t len, char *line, char *buff)
{
	char *str = NULL, *ln_cpy = line;

	str = malloc((sizeof(*str) * len) + 1);
	if (!str)
		return (NULL);

	str = _strncpy(str, buff, len);
	str[len] = '\0';
	line = str_concat(line, str);
	if (ln_cpy)
		free(ln_cpy);

	free(str);
	return (line);
}

/**
 * is_EOF - records EOF value from read
 * @seteof: an int used to set or check EOF
 *
 * Return: +ve int if EOF, 0 if false
 */
int is_EOF(int seteof)
{
	static int isEOF;

	if (seteof != -1)
		isEOF = seteof;

	return (isEOF);
}
