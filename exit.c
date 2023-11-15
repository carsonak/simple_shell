#include "shell.h"

/**
 * mk_func - changes the working dir to current shell executionenv
 * @args: target directory
 *
 * Return: 1 one success, 0 otherwise.
 **/
int mk_func(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "unexpected argument to \"cd\n");
	}
	else
	{
		if (chdir(args[1] != 0))
		{	
			perror("error in own_mk.c: changint the dir/n");
		}
	}
	return (-1);
}
