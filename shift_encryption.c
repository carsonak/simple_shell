#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * main - entry point
 *
 * Return: 0
 */
int main(void)
{
	int num = 23456789;
	void *c = &num;
	int blocks = sizeof(num) / sizeof(char);
	int sum = 0, i = 0;

	for (i = 0; i < blocks; i++)
		sum += (((unsigned char *)(c))[i] << (sizeof(char) * 8 * i));

	printf("Num: %u\nDigits: %u\n", sum, i);
	return (0);
}
