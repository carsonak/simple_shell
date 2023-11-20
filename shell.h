#ifndef _SHELL_H_
#define _SHELL_H_

#include <string.h>	  /*strtok(), strlen()*/
#include <stdio.h>	  /*perror(), dprintf()*/
#include <errno.h>	  /*perror(), errno global variables*/
#include <unistd.h>	  /*write(), fork(), execve(), read(), close()*/
#include <stdlib.h>	  /*exit()*/
#include <fcntl.h>	  /*open()*/
#include <signal.h>	  /*signal()*/
#include <dirent.h>	  /*opendir(), readdir()*/
#include <stdbool.h>  /*bool*/
#include <sys/types.h>/*wait(), waitpid(), fork(), open()*/
#include <sys/wait.h> /*wait(), waitpid()*/
#include <sys/stat.h> /*open()*/
#define BUFFER_SIZE (1024 * 1024)

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
long int _getline(char **line, ssize_t *ln_sz, int fd);
ssize_t byteRd(int fd, char *buff);
ssize_t find_line(char *buff, ssize_t crnt_i, ssize_t byt_cnt);
char *mem_line(char *line, int sz);
char *_strtok(char *str, char *delim);
char *_memset(char *s, char c, unsigned int n);
char *_strncpy(char *dest, char *src, int n);
char *_strdup(char *str);
int _strncmp(char *s1, char *s2, unsigned int n);
char *_strncat(char *dest, char *src, int n);
char *_getenv(char *name);
cmd_str *add_node_end(cmd_str **head, char *str);
void prompt(void);
void parse_n_exec(void);
int executor(char **cmds);
char **parser(char **cmds, char *line);
void free_list(cmd_str *head);
void flush_io(void);
void free_args(char **cmds);
char **cmds_fill(cmd_str *head, char **cmds);
int isPath(char **cmd);
int abs_search(char *path);
int searchDIR(char *dirPath, char *file);
int is_abs_path(char *cmd);
char *make_path(char *directory, char *file);
int E_status(int stat);

#endif /*_SHELL_H_*/
