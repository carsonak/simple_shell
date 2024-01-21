#ifndef _SHELL_H_
#define _SHELL_H_

#include "./Lib1-u_string/u_string.h" /*string lib replicas*/
#include <stdio.h>					/*perror(), dprintf()*/
#include <errno.h>					/*perror(), errno global variables*/
#include <unistd.h>					/*write(), fork(), execve(), read(), close()*/
#include <stdlib.h>					/*exit()*/
#include <fcntl.h>					/*open()*/
#include <signal.h>					/*signal()*/
#include <dirent.h>					/*opendir(), readdir()*/
#include <stdbool.h>				/*bool*/
#include <sys/types.h>				/*wait(), waitpid(), fork(), open()*/
#include <sys/wait.h>				/*wait(), waitpid()*/
#include <sys/stat.h>				/*open()*/

/*Buffer size for _getline*/
#define BUFFER_SIZE (1024 * 8)

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

/*Standard lib functions replicas*/

char *_memset(char *s, char c, unsigned int n);
ssize_t _getline(char **line, size_t *ln_sz, int fd);
ssize_t find_line(char *buff, ssize_t crnt_i, ssize_t byt_cnt);
char *line_alloc(ssize_t len, char *buff);
char *_getenv(char *name);
/*end*/

void inatty(void);
void notatty(void);
void prompt(void);

cmd_str *add_node_end(cmd_str **head, char *str);
void free_list(cmd_str *head);

char **parser(char *line);
int executor(char **cmds);

int isPath(char **cmd);
int is_abs_path(char *cmd);
int searchDIR(char **dirPath, char *file);
int abs_search(char **path);
char *append_dir(char *path, char *cwd);
char **cmd_fill(cmd_str *head, char **cmds);
char *make_path(char *directory, char *file);

void free_args(char **cmds);
int err_handler(char *prog_name, int stat, char *cmd);
int badbad(char *prog_name, char **msg, char *cnt_s, char *cmd, char *panic);

#endif /*_SHELL_H_*/
