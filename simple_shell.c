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
 * Return: 0 on success, 1 on error.
 */
static int process_input(FILE *input_stream)
{
	size_t lines_read = 1;
	ssize_t chars_read = 0;
	unsigned char using_terminal = 0;

	assert(input_stream);
	assert(fileno(input_stream) > -1);
	using_terminal = isatty(fileno(input_stream));
	for (lines_read = 1; chars_read >= 0; ++lines_read)
	{
		char *raw_line = NULL;
		queue *tokens = NULL;
		int ret_val = 0;

		(void)lines_read;
		if (using_terminal == 1)
			ECHOL(SIMPLE_SHELL_PROMPT, sizeof(SIMPLE_SHELL_PROMPT) - 1);

		chars_read = _getline(&raw_line, NULL, input_stream);
		if (chars_read < 0)
		{
			raw_line = _free(raw_line);
			if (chars_read == GETLINE_EOF)
				return (0);

			return (1);
		}

		if (raw_line[chars_read - 1] == '\n')
			raw_line[--chars_read] = '\0';

		tokens = tokenise(raw_line, chars_read);
		raw_line = _free(raw_line);
		if (!tokens)
			return (1);

		if (tokens->length > 0)
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
