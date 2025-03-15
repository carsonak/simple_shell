#include <assert.h>

#include "simple_shell.h"

/**
 * parse_argv - parse command line arguments.
 * @argc: number of arguments.
 * @argv: array of string pointers.
 */
static void parse_argv(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
}

/**
 * get_command - retrieve command from input_stream.
 * @line: pointer to a buffer to store the command.
 * @input_stream: pointer to an open stream.
 *
 * Return: number of characters in the command on success, EOF or -1 on error.
 */
static ssize_t get_command(char **const line, FILE *input_stream)
{
	ssize_t chars_read = 0;

	assert(line);
	assert(input_stream);
	chars_read = _getline(line, NULL, input_stream);
	if (chars_read < 0)
	{
		*line = _free(*line);
		if (chars_read == GETLINE_EOF)
			return (GETLINE_EOF);

		return (-1);
	}

	if ((*line)[chars_read - 1] == '\n')
		(*line)[--chars_read] = '\0';

	return (chars_read);
}

/**
 * process_input - read and execute commands from an input stream.
 * @input_stream: the input stream.
 * @environ: pointer to an `environment_vars` struct.
 *
 * Return: 0 on success, 1 on error.
 */
static int process_input(FILE *input_stream, environment_vars * const environ)
{
	size_t lines_read = 1;
	ssize_t chars_read = 0;
	unsigned char using_terminal = 0;

	assert(input_stream);
	assert(fileno(input_stream) > -1);
	using_terminal = isatty(fileno(input_stream));
	for (lines_read = 1; chars_read >= 0; ++lines_read)
	{
		char *line = NULL;
		queue *tokens = NULL;
		int ret_val = 0;

		(void)lines_read;
		if (using_terminal == 1)
			ECHOL(SIMPLE_SHELL_PROMPT, sizeof(SIMPLE_SHELL_PROMPT) - 1);

		chars_read = get_command(&line, input_stream);
		if (chars_read == GETLINE_EOF)
			return (0);

		if (chars_read < 0)
			return (1);

		tokens = tokenise(line, chars_read);
		line = _free(line);
		if (!tokens)
			return (1);

		if (tokens->length > 0)
			ret_val = interprate(tokens, environ);

		tokens = queue_delete(tokens, string_delete);
		if (ret_val < 0)
			return (ret_val);
	}

	return (0);
}

/**
 * dup_str - wrapper for `_strdup`.
 * @data: a character pointer.
 *
 * Return: the duplicated string.
 */
static void *dup_str(void const *const data)
{
	const char *const str = data;

	return (_strdup(str, _strlen(str)));
}

/**
 * main - Entry point
 * @argc: number of arguments passed to the program.
 * @argv: pointer to the strings of command line arguments.
 * @envp: array of environment variables.
 *
 * Return: 0
 */
int main(int argc, char *argv[], char *envp[])
{
	int ret_val = 0;
	environment_vars environ = {0};

	for (environ.len = 0; envp[environ.len]; ++environ.len)
		;

	environ.env = (char **)dup_2D_array(
		(void **)envp, environ.len, dup_str, free);
	parse_argv(argc, argv);
	ret_val = process_input(stdin, &environ);
	environ.env = delete_2D_array((void **)environ.env, environ.len, free);
	return (ret_val);
}
