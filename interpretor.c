#include "simple_shell.h"
#include "shell_types.h"
#include <assert.h>

/**
 * get_shell_variable - extract a `shell_variable` type from a string.
 * @str: the string to read from.
 *
 * Return: a `shell_variable` type with both the name and value on success,
 * a NULL `shell_variable` type on failure.
 */
static shell_variable get_shell_variable(view_string str)
{
	view_string t = {0};
	shell_variable var = {0};

	assert(str.s && str.size > 0);
	str.i = 0;
	t = _strtok(&str, "=");
	if (!t.s)
		return (var);

	var.name = _strdup(t.s, t.size);
	if (!var.name)
		return (var); /* MALLOC FAIL */

	t.s += t.size + 1;
	t.size = str.size - (t.size + 1);
	var.value = _strdup(t.s, t.size);
	if (!var.value)
		var.name = _free(var.name); /* MALLOC FAIL */

	return (var);
}

/**
 * get_shell_command - extract a `shell_command` type from a `queue` of tokens.
 * @tokens: `queue` of strings.
 *
 * Return: a `shell_command` type with the command name and its arguments on
 * success, a NULL `shell_command` type on failure.
 */
static shell_command get_shell_command(queue *tokens)
{
	view_string *tok = NULL;
	shell_command cmd = {0};
	intmax_t argv_i = 0;

	assert(tokens);
	tok = dequeue(tokens);
	if (!tok)
		return (cmd);

	cmd.cmd = _strdup(tok->s, tok->size);
	if (!cmd.cmd) /* MALLOC FAIL */
	{
		cmd.cmd = _free(cmd.cmd);
		return (cmd);
	}

	if (tokens->length < 1)
		return (cmd);

	cmd.argv = queue_to_array(tokens, string_to_cstr, _free);
	if (!cmd.argv)
		cmd.cmd = _free(cmd.cmd);

	return (cmd);
}

/**
 * interprate - execute command in tokens.
 * @tokens: list of tokens from input.
 *
 * Return: -1 on error.
 */
int interprate(queue *tokens)
{
	view_string *tok = NULL;
	queue shell_vars = {0};
	shell_variable var = {0};
	shell_command cmd = {0};

	if (!tokens)
		return (-1);

	tok = dequeue(tokens);
	while (tok && _strstr(tok->s, "=").s)
	{
		var = get_shell_variable(*tok);
		if (var.name)
			enqueue(&shell_vars, &var, NULL);

		tok = dequeue(tokens);
	}

	cmd = get_shell_command(tokens);
	if (cmd.cmd)
	{
		if (shell_vars.length > 0)
		/* Add variable */
	}
}
