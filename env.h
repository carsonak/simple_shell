#ifndef SIMPLE_SHELL__ENV_H
#define SIMPLE_SHELL__ENV_H

#include <inttypes.h>

#include "shell_types.h"
#include "strings.h"
#include "alloc.h"

/**
 * struct environment_vars - struct for holding environment variables.
 * @len: number of variables in the array.
 * @env: array of pointers to environment variable strings.
 */
struct environment_vars
{
	intmax_t len;
	char **env;
};

typedef struct environment_vars environment_vars;

#define SETENV_INVALID_NAME -2

intmax_t _getenv(
	const environment_vars * const environ, const char * const name);
int _setenv(environment_vars * const environ, view_string * const var);

#endif /* SIMPLE_SHELL__ENV_H */
