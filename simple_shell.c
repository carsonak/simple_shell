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
 * process_input - read and execute commands from an input stream.
 * @input_stream: the input stream.
 *
 * Return: -1 on error, 0 otherwise.
 */
static int process_input(FILE *input_stream)
{
	size_t lines_read = 1;
	ssize_t chars_read = 0;
	unsigned char using_terminal = 0;
	int input_fd = fileno(input_stream);

	assert(input_stream);
	if (input_fd < 0)
		return (-1);

	using_terminal = isatty(input_fd);
	for (lines_read = 1; chars_read >= 0; ++lines_read)
	{
		char *raw_line = NULL;
		size_t raw_line_size = 0;
		queue *tokens = NULL;
		int ret_val = 0;

		(void)lines_read;
		if (using_terminal == 1)
			ECHOL(SIMPLE_SHELL_PROMPT, sizeof(SIMPLE_SHELL_PROMPT) - 1);

		errno = 0;
		chars_read = getline(&raw_line, &raw_line_size, input_stream);
		if (chars_read < 0)
		{
			raw_line = _free(raw_line);
			if (errno)
			{
				perror("ERROR: process_input");
				return (-1);
			}

			return (0);
		}

		if (raw_line[chars_read - 1] == '\n')
		{
			--chars_read;
			raw_line[chars_read] = '\0';
		}

		tokens = tokenise(raw_line, chars_read);
		raw_line = _free(raw_line);
		if (!tokens)
			return (-1);

		ret_val = interprate(tokens);
		tokens = queue_delete(tokens, string_delete);
		if (ret_val < 0)
			return (ret_val);
	}

	return (0);
}

static void *dup_str(void const *const data)
{
	const char *const str = data;

	return (_strdup(str, _strlen(str)));
}

/**
 * main - Entry point
 * @argc: number of arguments passed to the program.
 * @argv: pointer to the strings of command line arguments.
 *
 * Return: 0
 */
int main(int argc, char *argv[])
{
	int ret_val = 0;
	intmax_t env_len = get_env_len();
	char **env_dup = (char **)dup_2D_array((void **)environ, env_len, dup_str, free);

	environ = env_dup;
	parse_argv(argc, argv);
	ret_val = process_input(stdin);
	environ = delete_2D_array((void **)environ, env_len, free);
	return (ret_val);
}
