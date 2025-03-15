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

	assert(str.s);
	assert(str.size > 0);
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

	assert(cmd);
	// assert(argv);
	assert(envp);
	if (pid == 0)
	{
		if (execve(cmd, argv, envp) < 0)
			perror("ERROR: " __FILE__ ":exec_command");

		exit(EXIT_FAILURE);
	}

	if (pid < 0)
	{
		perror("ERROR: " __FILE__ ":exec_command");
		return (0);
	}

	if (waitpid(pid, &child_status, WUNTRACED) < 0)
	{
		perror("ERROR: " __FILE__ ":exec_command");
		return (0);
	}

	if (WIFSIGNALED(child_status) ||
		(WIFEXITED(child_status) && WEXITSTATUS(child_status) != 0))
		return (0);

	return (1);
}

void free_shell_variable(void *const data)
{
	shell_variable *v = data;

	if (v)
	{
		v->name = _free(v->name);
		v->value = _free(v->value);
	}
}

/**
 * interprate - execute command in tokens.
 * @tokens: list of tokens from input.
 *
 * Return: 0 on success, -1 on error.
 */
int interprate(queue *const tokens)
{
	queue shell_vars = {0};
	shell_variable *var_ptr = NULL;
	char **argv = NULL;
	int return_val = 0;

	if (!tokens)
		return (-1);

	while (tokens->head && _strstr(((view_string *)tokens->head->data)->s, "=").s)
	{
		view_string *tok = dequeue(tokens);
		shell_variable var = get_shell_variable(*tok);

		string_delete(tok);
		if (!var.name)
		{
			return_val = -1;
			goto clean_exit;
		}

		enqueue(&shell_vars, &var, NULL);
	}

	argv = (char **)queue_to_array(tokens, string_to_cstr, free);
	if (!argv)
	{
		return_val = -1;
		goto clean_exit;
	}

	var_ptr = dequeue(&shell_vars);
	while (var_ptr)
	{
		int setenv_ret = _setenv(*var_ptr);

		free_shell_variable(var_ptr);
		if (setenv_ret < 0)
		{
			return_val = -1;
			goto clean_exit;
		}

		var_ptr = dequeue(&shell_vars);
	}

	if (exec_command(argv[0], argv, environ) == 0)
		return_val = -1;

clean_exit:
	argv = delete_2D_array((void **)argv, tokens->length, free);
	queue_clear(&shell_vars, free_shell_variable);
	return (return_val);
}
