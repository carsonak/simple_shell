#include "simple_shell.h"
#include "shell_types.h"
#include <assert.h>

/**
 * get_shell_variables - identify and retrieve shell variables from a
 * list of tokens.
 * @shell_vars: queue for storing the shell variables.
 * @tokens: the tokens to check.
 */
static void get_shell_variables(queue *shell_vars, queue *tokens)
{
	assert(shell_vars);
	assert(tokens);
	while (tokens->head &&
		   _strstr(((view_string *)queue_peek_head(tokens))->s, "=").s)
		enqueue(shell_vars, dequeue(tokens), NULL);
}

/**
 * export_shell_variables - export the given variables to the environment.
 * @shell_vars: list of shell variables to export.
 * @environ: pointer to an `environment_vars` struct.
 *
 * Return: 0 on success, -1 on error.
 */
static int export_shell_variables(queue *shell_vars, environment_vars *environ)
{
	view_string *var_ptr = dequeue(shell_vars);

	while (var_ptr)
	{
		int setenv_ret = _setenv(environ, var_ptr);

		string_delete(var_ptr);
		if (setenv_ret < 0)
			return (-1);

		var_ptr = dequeue(shell_vars);
	}

	return (0);
}

/**
 * exec_command - fork and call execve on `cmd`.
 * @cmd: path to file to execute.
 * @argv: arguments to the command to execute.
 * @envp: environment variables to the command.
 *
 * Return: 0 on success, -1 on failure.
 */
static int exec_command(
	const char *const cmd, char *const *const argv, char *const *const envp)
{
	int child_status = 0;
	pid_t pid = fork();

	if (pid == 0)
	{
		if (execve(cmd, argv, envp) < 0)
			perror("ERROR: " __FILE__ ":exec_command");

		exit(EXIT_FAILURE);
	}

	if (pid < 0)
	{
		perror("ERROR: " __FILE__ ":exec_command");
		return (-1);
	}

	if (waitpid(pid, &child_status, WUNTRACED) < 0)
	{
		perror("ERROR: " __FILE__ ":exec_command");
		return (-1);
	}

	if (WIFSIGNALED(child_status) ||
		(WIFEXITED(child_status) && WEXITSTATUS(child_status) != 0))
		return (-1);

	return (0);
}

/**
 * interprate - execute command in tokens.
 * @tokens: list of tokens from input.
 * @environ: pointer to an `environment_vars` struct.
 *
 * Return: 0 on success, -1 on error.
 */
int interprate(queue * const tokens, environment_vars * const environ)
{
	queue shell_vars = {0};
	char **argv = NULL;
	int ret_val = 0;

	if (!tokens)
		return (-1);

	get_shell_variables(&shell_vars, tokens);
	argv = (char **)queue_to_array(tokens, string_to_cstr, free);
	if (!argv)
	{
		ret_val = -1;
		goto clean_exit;
	}

	if (export_shell_variables(&shell_vars, environ) < 0)
	{
		ret_val = -1;
		goto clean_exit;
	}

	if (exec_command(argv[0], argv, environ->env) < 0)
		ret_val = -1;

clean_exit:
	argv = delete_2D_array((void **)argv, tokens->length, free);
	queue_clear(&shell_vars, string_delete);
	return (ret_val);
}
