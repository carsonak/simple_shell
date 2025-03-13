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

	cmd.argv = (char **)queue_to_array(tokens, string_to_cstr, free);
	if (!cmd.argv) /* MALLOC FAIL */
		cmd.cmd = _free(cmd.cmd);

	return (cmd);
}

/**
 * exec_command - fork and call execve on `cmd`.
 * @cmd: path to file to execute.
 * @argv: arguments to the command to execute.
 * @envp: environment variables to the command.
 *
 * Return: 1 on success, 0 on failure.
 */
static unsigned char exec_command(
	const char *const cmd, char *const *const argv, char *const *const envp)
{
	int child_status = 0;
	pid_t pid = fork();

	if (pid == 0)
	{
		if (execve(cmd, argv, envp) < 0)
			perror("ERROR: exec_command");

		exit(EXIT_FAILURE);
	}

	if (pid < 0)
	{
		perror("ERROR: exec_command");
		return (0);
	}

	if (waitpid(pid, &child_status, WUNTRACED) < 0)
	{
		perror("ERROR: exec_command");
		return (0);
	}

	if (WIFSIGNALED(child_status) ||
		(WIFEXITED(child_status) && WEXITSTATUS(child_status) != 0))
		return (0);

	return (1);
}

/**
 * interprate - execute command in tokens.
 * @tokens: list of tokens from input.
 *
 * Return: 0 on success, -1 on error.
 */
int interprate(queue *tokens)
{
	view_string *tok = NULL;
	queue shell_vars = {0};
	shell_variable var = {0}, *var_ptr = NULL;
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
	var_ptr = dequeue(&shell_vars);
	while (var_ptr)
	{
		if (_setenv(*var_ptr) < 0)
			return (-1);

		var_ptr = dequeue(&shell_vars);
	}

	if (exec_command(cmd.cmd, cmd.argv, environ) == 0)
		return (-1);

	return (0);
}
