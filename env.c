#include <assert.h>

#include "env.h"

/**
 * _getenv - searches the environment variables for `name`.
 * @environ: pointer to an `environment_vars` struct.
 * @name: name of the environment variable to look for.
 *
 * Return: index of the environment variable if found, -1 otherwise.
 */
intmax_t _getenv(
	const environment_vars * const environ, const char * const name)
{
	intmax_t env_i = 0;
	intmax_t name_len = _strlen(name);

	if (name_len < 1)
		return (-1);

	for (; environ->env[env_i]; ++env_i)
	{
		if (_strncmp(environ->env[env_i], name, name_len) == 0)
			return (env_i);
	}

	return (-1);
}

/**
 * strsplit - splits a string at the given character and returns first
 * section of the string.
 * @str: the string to split.
 * @c: character to split at.
 *
 * Return: a string type with the first section of the string,
 * NULL `string` type on error.
 */
static string strsplit(const char *const str, char c)
{
	intmax_t var_len = 0;
	string var_name = {0};

	assert(str);
	assert(c > 0);
	for (var_len = 0;
		 str[var_len] != '\0' && str[var_len] != c; ++var_len)
		;

	if (str[var_len] == '\0')
		return (var_name);

	var_name.size = var_len;
	var_name.s = _strdup(str, var_len);
	return (var_name);
}

/**
 * _setenv - exports a shell variable to the environment.
 * @environ: pointer to an `environment_vars` struct.
 * @var: a shell variable of the form `name=value`.
 *
 * Return: 0 on success, -1 on malloc on fail, -2 on invalid variable name.
 */
int _setenv(environment_vars * const environ, view_string * const var)
{
	intmax_t insert_index = -1;
	char *env_var = NULL, **resized_environ = NULL;
	string var_name = {0};

	if (!var || !var->s || var->size < 1 || !_strstr(var->s, "=").s)
		return (SETENV_INVALID_NAME);

	var_name = strsplit(var->s, '=');
	if (!var_name.s)
		return (-1);

	env_var = (char *)var->s;
	var->s = NULL;
	insert_index = _getenv(environ, var_name.s);
	var_name.s = _free(var_name.s);
	if (insert_index > -1)
		_free(environ->env[insert_index]);
	else
	{
		insert_index = environ->len;
		resized_environ = _realloc(
			environ->env, sizeof(*resized_environ) * (insert_index + 2),
			sizeof(*resized_environ) * insert_index);
		if (!resized_environ)
			return (-1);

		++environ->len;
		resized_environ[insert_index + 1] = NULL;
	}

	resized_environ[insert_index] = env_var;
	environ->env = resized_environ;
	return (0);
}
