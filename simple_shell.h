#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

/* getline, fileno */
#define _POSIX_C_SOURCE 200809L

#include <inttypes.h> /* intmax_t */
#include <stdio.h>	  /* getline, perror, FILE, fileno */
#include <sys/wait.h> /* wait, waitpid */
#include <unistd.h>   /* fork, getpid, execve */
#include <errno.h>    /* errno */

#include "alloc.h"
#include "queue.h"
#include "env.h"

#define SIMPLE_SHELL_PROMPT "@s_s> "
#define GETLINE_EOF -2
/* print text of the specified length to stdout. */
#define ECHOL(echo_txt, echo_txt_len) \
	write(STDOUT_FILENO, echo_txt, echo_txt_len)
/* print text to stdout. */
#define ECHO(echo_txt) write(STDOUT_FILENO, echo_txt, _strlen(echo_txt))

ssize_t _getline(char **const lineptr, size_t * const n, FILE * const stream);
queue *tokenise(const char *const command_str, intmax_t size);
int interprate(queue *const tokens, environment_vars *environ);

#endif /* SIMPLE_SHELL_H */
