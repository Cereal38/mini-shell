/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "utils.h"

int main()
{

  Signal(SIGINT, handler_interrupt_shell); // Ignore the signal Ctrl+c
  // Variables to set the prompt
  char *path;
  char *user = getenv("USER");

  char *commande;

  while (1)
  {

    // Get the path of thhe current directory
    path = getcwd(NULL, 0);
    struct cmdline *l;

    // Display the prompt
    printf("%s%s%s:%s%s%s$ ", GREEN, user, RESET, BLUE, path, RESET);
    l = readcmd();

    // Ctrl+d
    if (!l)
    {
      printf("\n");
      exit(0);
    }

    // Avoid segfault if the user leaves the prompt empty
    if (l != NULL && l->seq[0] != NULL)
    {
      commande = l->seq[0][0];

      if (l->err)
      {
        /* Syntax error, read another command */
        printf("error: %s\n", l->err);
        continue;
      }

      if (is_internal(commande))
      {
        exec_internal(l);
      }
      else
      {
        exec_external(l);
      }
    }
  }
}
