
#include "shell.h"

/**
 * exit : function that terminate the prog when user type "exit"
 */
void sh_exit(void)
{
	printf("Exit the shell \n");
	exit(0);
}
int main(void)
{
	char input[MAX_INPUT];

	while(1)
	{
		printf("cisfun ");
		fgets (input, sizeof(input), stdin);

		if (strcmp(input, "exit") == 0)
		{
			sh_exit();
		}
	system(input);
	}
	return (0);
}
