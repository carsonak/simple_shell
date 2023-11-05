#ifndef _SCRAP_H_
#define _SCRAP_H_

#include <string.h>/*strtok(), strlen()*/
#include <stdio.h> /*perror(), dprintf()*/
#include <errno.h> /*perror(), errno global variables*/
#include <unistd.h>
/**
 * write(), fork(), execve(), read(), close()
 * std exit statuses, std file descriptors
 */
#include <stdlib.h>   /*exit()*/
#include <fcntl.h>    /*open()*/
#include <sys/types.h>/*wait(), waitpid(), fork(), open()*/
#include <sys/wait.h> /*wait(), waitpid()*/
#include <sys/stat.h> /*open()*/

int executor(char **cmds);

#endif /*_SCRAP_H_*/
