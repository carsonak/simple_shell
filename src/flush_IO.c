#include "shell.h"

/**
 * flush_io - flushes input/output
 */
void flush_io(void)
{
	int err = 0;

	err = fflush(stdin);
	if (err != 0)
	{
		perror("Couldn't flush stdin");
		err = 0;
	}

	err = fflush(stdout);
	if (err != 0)
	{
		perror("Couldn't flush stdout");
		err = 0;
	}

	err = fflush(stderr);
	if (err != 0)
	{
		perror("Couldn't flush stderr");
		err = 0;
	}
}
