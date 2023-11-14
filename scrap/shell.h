#ifndef _SHELL_H_
#define _SHELL_H_

#include <string.h>	  /*strtok(), strlen()*/
#include <stdio.h>	  /*perror(), dprintf()*/
#include <errno.h>	  /*perror(), errno global variables*/
#include <unistd.h>	  /*write(), fork(), execve(), read(), close(), std exit statuses, std file descriptors*/
#include <stdlib.h>	  /*exit()*/
#include <fcntl.h>	  /*open()*/
#include <sys/types.h>/*wait(), waitpid(), fork(), open()*/
#include <sys/wait.h> /*wait(), waitpid()*/
#include <sys/stat.h> /*open()*/
#define BUFFER_SIZE (1024 * 5)

/**
 * struct command_string_list - linked list of a command and it's options
 * @command: null terminated string
 * @len: length of the command
 * @next: pointer to next node
 */
typedef struct command_string_list
{
	char *command;
	unsigned int len;
	struct command_string_list *next;
} cmd_str;

unsigned long int _strlen(char *s);
unsigned int _strspn(char *s, char *accept);
int _getline(char **line, int *n_c, int fd);
char *_strtok(char *str, char *delim);
char *_memset(char *s, char c, unsigned int n);
char *_strncpy(char *dest, char *src, int n);
char *_strdup(char *str);
cmd_str *add_node_end(cmd_str **head, char *str);
int executor(char **cmds);
char **parser(char **cmds);
void free_list(cmd_str *head);
void flush_io(void);
void free_args(char **cmds);
char **cmds_fill(cmd_str *head, char **cmds);

#endif /*_SHELL_H_*/
