#ifndef _SHELL_H_
#define _SHELL_H_

#include "u_string.h" /*string lib replicas*/
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
long int _getline(char **line, ssize_t *ln_sz, int fd);
ssize_t find_line(char *buff, ssize_t crnt_i, ssize_t byt_cnt);
char *line_alloc(ssize_t len, char *buff);
char *_getenv(char *name);
/*end*/

char *lintos(ssize_t num);
cmd_str *add_node_end(cmd_str **head, char *str);
void prompt(void);
ssize_t parse_n_exec(void);
int executor(char **cmds);
char **parser(char *line);
void free_list(cmd_str *head);
void flush_io(void);
void free_args(char **cmds);
char **cmd_fill(cmd_str *head, char **cmds);
int isPath(char **cmd);
int abs_search(char **path);
char *append_dir(char *path, char *cwd);
int searchDIR(char **dirPath, char *file);
char *trim_str(char *str, int cut);
int is_abs_path(char *cmd);
char *make_path(char *directory, char *file);
int err_handler(char *prog_name, int stat, char *cmd);
int badbad(char *prog_name, char **msg, char *cnt_s, char *cmd, char *panic);

#endif /*_SHELL_H_*/
