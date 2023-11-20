#include "shell.h"

/**
 * E_status - records an exit status of the program.
 * @stat: the exit status
 *
 * Return: the stored exit status
 */
int E_status(int stat)
{
	static int err;

	if (stat != 1000)
		err = stat;

	return (err);
}
