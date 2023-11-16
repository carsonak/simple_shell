#include "shell.h"

int main(int ac, char **av, char **env)
{
  int a = 0;
  while(env[a] != NULL)
  { 
    printf("env[%d] = %s\n", a, env[a]);
    a++;
  }
  return (0);
}
