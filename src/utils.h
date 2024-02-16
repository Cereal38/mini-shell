#ifndef __UTILS_H
#define __UTILS_H

#include "readcmd.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2
  
/*
Return true if the command is internal

Input:
  cmd: string (Command to analyze)
    
Output:
  1 if the command is an internal command
  0 if not
*/
int is_internal (char *cmd);

/*
    Exec an internal command

    Input:
      l: cmdline* (Current cmdline)
*/
void exec_internal (struct cmdline *l);

/*
    Exec an external command in a child process and wait for it to finish in the parent process

    Input:
      l: cmdline* (Current cmdline)
*/
void exec_external (struct cmdline *l);

/*
    Redirect input and output to the given file

    Input:
      l: cmdline* (Current cmdline)

*/
void redirectInOut (struct cmdline *l);

/*
    Print an error message to stderr depending on the error code

    Input:
      commande: string (Command that caused the error)
*/
void gestion_erreur(char *commande);
#endif