#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

/* getline */
#define _POSIX_C_SOURCE 200809L

#include <inttypes.h> /* intmax_t */
#include <stdio.h>    /* getline */
#include <sys/wait.h> /* wait */
#include <unistd.h>   /* fork, getpid */

#include "alloc.h"
#include "singly_linked_list.h"
#include "queue.h"
#include "string.h"

#define SIMPLE_SHELL_PROMPT "@s_s> "

queue *tokenise(const char *const command_str, intmax_t size);
void interprate(queue *tokens);

#endif /* SIMPLE_SHELL_H */
