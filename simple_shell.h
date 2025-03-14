#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

/* getline */
#define _POSIX_C_SOURCE 200809L

#include <inttypes.h> /* intmax_t */
#include <stdio.h>    /* getline, perror */
#include <sys/wait.h> /* wait, waitpid */
#include <unistd.h>   /* fork, getpid, execve */
#include <errno.h>    /* errno */

#include "alloc.h"
#include "queue.h"
#include "_env.h"

#define SIMPLE_SHELL_PROMPT "@s_s> "
#define ECHOL(echo_str, echo_str_len) write(STDOUT_FILENO, echo_str, echo_str_len)
#define ECHO(echo_str) write(STDOUT_FILENO, echo_str, _strlen(echo_str))

queue *tokenise(const char *const command_str, intmax_t size);
int interprate(queue *const tokens);

#endif /* SIMPLE_SHELL_H */
