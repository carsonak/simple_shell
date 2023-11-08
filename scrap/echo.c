#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * main - echos stdin to stdout
 *
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
	char buf[1024];
	int err;

	while (1)
	{
		err = write(1, "$! ", 2);
		if (err == -1)
		{
			perror("Whoops");
			return (EXIT_FAILURE);
		}
		err = read(0, buf, 1024);
		if (err == -1)
		{
			perror("Failed to read");
			return (EXIT_FAILURE);
		}

		err = write(1, buf, strlen(buf));
		if (err == -1)
		{
			perror("Failed to write");
			return (EXIT_FAILURE);
		}

		memset(buf, '\0', 1024);
	}

	return (EXIT_SUCCESS);
}
