#include <assert.h>

#include "simple_shell.h"

#define GETLINE_BUFFER_SIZE 1024U

/**
 * read_into_buffer - read from file descriptor `fd` a number of bytes into a
 * buffer.
 * @buffer: the buffer to write to.
 * @fd: an open file descriptor.
 *
 * Return: return value of the read function.
 */
static ssize_t read_into_buffer(string *buffer, const int fd)
{
	ssize_t characters_read = 0;

	assert(buffer);
	assert(buffer->s);
	assert(fd > -1);
	buffer->i = 0;
	characters_read = read(fd, buffer->s, GETLINE_BUFFER_SIZE);
	if (characters_read < 0)
		perror("ERROR: " __FILE__ ":read_into_buffer");

	buffer->size = characters_read;
	return (characters_read);
}

/**
 * getline_from_buffer - read a single line from `buffer` into `dest`.
 * @buffer: pointer to the buffer.
 * @dest: pointer to string type to store the line.
 *
 * Return: number of characters in the extracted line, -1 on error.
 */
ssize_t getline_from_buffer(view_string * const buffer, string * const dest)
{
	ssize_t line_size = 0;
	char c = 0;

	assert(buffer);
	assert(buffer->s);
	assert(buffer->size > 0);
	assert(buffer->i > -1);
	assert(dest);
	assert(dest->s);
	assert(dest->size > 0);
	for (c = string_readc(buffer); c > -1 && c != '\n';
		 ++line_size, c = string_readc(buffer))
		;

	if (c > -1)
		++line_size;

	if (dest->size <= line_size)
	{
		char *line = _malloc(line_size + 1);

		if (!line)
			return (-1);

		_free(dest->s);
		dest->s = line;
	}

	dest->size = line_size;
	_memcpy(dest->s, &buffer->s[buffer->i - line_size], line_size);
	dest->s[line_size] = '\0';
	return (line_size);
}

/**
 * _getline - reads an entire line from `fd`. Behaves similary to the
 * @lineptr: address of where to store text read.
 * @n: size of `*lineptr`, if `*lineptr` was pre-malloced.
 * @stream: open stream to read from.
 *
 * Return: number of characters read, -2 on EOF, -1 on error.
 */
ssize_t _getline(char **const lineptr, size_t * const n, FILE * const stream)
{
	static char buffer[GETLINE_BUFFER_SIZE];
	static string buf_string;
	string line = {0};
	ssize_t line_length = 0;
	const int fd = fileno(stream);

	if (!lineptr || (*lineptr && !n))
		return (-1);

	if (!*lineptr)
	{
		line.size = 32;
		*lineptr = _calloc(sizeof(**lineptr), line.size + 1);
	}

	line.s = *lineptr;
	if (fd < 0)
		return (-1);

	if (line.size == 0)
		line.size = *n;

	buf_string.s = buffer;
	if (buf_string.size < 1 || buf_string.i >= buf_string.size)
		if (read_into_buffer(&buf_string, fd) < 0)
			return (-1);

	if (buf_string.size == 0)
		return (GETLINE_EOF);

	line_length = getline_from_buffer((view_string *)&buf_string, &line);
	*lineptr = line.s;
	return (line_length);
}
