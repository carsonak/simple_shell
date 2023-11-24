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
#include <stdarg.h>	  /*variadic functions*/
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

/*standard lib functions replicas*/
size_t _strlen(char *s);
size_t _strspn(char *s, char *accept);
size_t _strcspn(char *s, char *reject);
long int _getline(char **line, ssize_t *ln_sz, int fd);
ssize_t find_line(char *buff, ssize_t crnt_i, ssize_t byt_cnt);
char *line_alloc(ssize_t len, char *line, char *buff);
char *_strtok(char *str, char *delim);
char *_memset(char *s, char c, unsigned int n);
char *_strncpy(char *dest, char *src, int n);
char *_strdup(char *str);
int _strncmp(char *s1, char *s2, size_t n);
char *_strncat(char *dest, char *src, int n);
char *_getenv(char *name);
/*end*/
char *stringscat(size_t items, ...);
char *str_concat(char *s1, char *s2);
char *lintos(ssize_t num);
int is_EOF(int seteof);
cmd_str *add_node_end(cmd_str **head, char *str);
void prompt(void);
ssize_t parse_n_exec(void);
int executor(char **cmds);
char **parser(char **cmds, char *line);
void free_list(cmd_str *head);
void flush_io(void);
void free_args(char **cmds);
char **cmds_fill(cmd_str *head, char **cmds);
int isPath(char **cmd);
int abs_search(char **path);
int rel_path(char **path);
int searchDIR(char **dirPath, char *file);
char *trim_str(char *str, int cut);
int is_abs_path(char *cmd);
char *make_path(char *directory, char *file);
int E_status(char *prog_name, int stat, char *cmd);

#endif /*_SHELL_H_*/
