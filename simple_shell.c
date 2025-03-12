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

	if (input_fd < 0)
		return (-1);

	using_terminal = isatty(input_fd);
	for (lines_read = 1; chars_read >= 0; ++lines_read)
	{
		char *raw_line = NULL;
		size_t raw_line_size = 0;
		queue *tokens = NULL;

		(void)lines_read;
		if (using_terminal == 1)
			printf(SIMPLE_SHELL_PROMPT);

		chars_read = getline(&raw_line, &raw_line_size, input_stream);
		if (chars_read < 0)
		{
			raw_line = _free(raw_line);
			return (-1);
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

		interprate(tokens);
		tokens = queue_delete(tokens, string_delete);
	}

	return (0);
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
	parse_argv(argc, argv);
	return (process_input(stdin));
}
