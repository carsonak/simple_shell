#include "_env.h"

/**
 * get_env_len - returns number of variables in the environment.
 *
 * Return: number of variables in the environment.
 */
intmax_t get_env_len(void)
{
	intmax_t i = 0;

	for (; environ[i]; ++i)
		;

	return (i);
}

/**
 * _getenv - searches the environment variables for `name`.
 * @name: name of the environment variable to look for.
 *
 * Return: index of the environment variable if found, -1 otherwise.
 */
intmax_t _getenv(const char *const name)
{
	intmax_t env_i = 0;
	intmax_t name_len = _strlen(name);

	if (name_len < 1)
		return (-1);

	for (; environ[env_i]; ++env_i)
	{
		if (_strncmp(environ[env_i], name, name_len) == 0)
			return (env_i);
	}

	return (-1);
}

/**
 * _setenv - exports `var` to the environment.
 * @var: a shell variable.
 *
 * Return: 0 on success, -1 on malloc on fail, -2 on invalid variable name.
 */
int _setenv(shell_variable var)
{
	intmax_t name_len = _strlen(var.name);
	intmax_t value_len = _strlen(var.value);
	intmax_t insert_index = -1;
	char *env_var = NULL, **resized_environ = NULL;

	if (!var.name || name_len < 1 || _strstr(var.name, "=").s)
		return (-2);

	env_var = _malloc(name_len + 1 + value_len + 1);
	if (!env_var)
		return (-1);

	_memcpy(env_var, var.name, name_len);
	_memcpy(env_var + name_len, "=", 1);
	_memcpy(env_var + name_len + 1, var.value, value_len);
	env_var[name_len + 1 + value_len] = '\0';
	insert_index = _getenv(var.name);
	if (insert_index > -1)
	{
		_free(environ[insert_index]);
		environ[insert_index] = env_var;
		return (0);
	}

	insert_index = get_env_len();
	resized_environ = _realloc(
		environ, sizeof(*resized_environ) * (insert_index + 1));
	if (!resized_environ)
		return (-1);

	resized_environ[insert_index] = env_var;
	resized_environ[insert_index + 1] = NULL;
	environ = resized_environ;
	return (0);
}
