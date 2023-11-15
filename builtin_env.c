#include "shell.h"

/**
 * shell_env :prints all the environment variables.
 *description :This function iterates through the 'environ' array, which is a global
 * variable in C containing the environment variables. For each variable,
 * it uses a custom function '_put' to print it, followed by a newline
 * return: 0 success
 **/
int shell_env(void)
{
	int i;

	for(i = 0; environ[i]; i++)
	{
		_put(environ[i];
		_putchar('\n');
	}
	return (0);
}
