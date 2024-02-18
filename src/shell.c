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
  // Variables to set the prompt
  char *path;
  char *user = getenv("USER");

  char *commande;

  while (1)
  {

    // Remember the path of thhe current directory
    path = getcwd(NULL, 0);
    struct cmdline *l;

    // Display the prompt
    printf("%s:%s$ ", user, path);
    l = readcmd();

    // Avoid segfault if the user leaves the prompt empty
    if (l != NULL && l->seq[0] != NULL)
    {
      commande = l->seq[0][0];

      /* If input stream closed, normal termination */
      if (!l)
      { // Ctrl+d
        printf("exit\n");
        exit(0);
      }

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
