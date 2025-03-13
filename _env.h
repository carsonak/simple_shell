#ifndef SIMPLE_SHELL__ENV_H
#define SIMPLE_SHELL__ENV_H

#include "shell_types.h"
#include "strings.h"
#include "alloc.h"

/* Environment variables */
extern char **environ;

intmax_t get_env_len(void);
intmax_t _getenv(const char *const name);
int _setenv(shell_variable var);

#endif /* SIMPLE_SHELL__ENV_H */
