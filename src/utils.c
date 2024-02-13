#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "utils.h"

int is_internal (char *cmd) {
  /*
    Input:
      cmd: string
    
    Output:
      1 if the command is an internal command
      0 if not
  */
  
  if (strcmp(cmd, "quit") == 0) {
    return 1;
  }
  else if (strcmp(cmd, "cd") == 0) {
    return 1;
  } 

  return 0;
}

void exec_internal(struct cmdline *l){


  if(strcmp(l->seq[0][0], "quit") == 0){
    exit(0);
  }

  else if(strcmp(l->seq[0][0], "cd") == 0){
    if(chdir(l->seq[0][1]) != 0){
      printf("Error: %s\n", strerror(errno));
    }
  }
}

void exec_external(struct cmdline *l){
  char *commande = l->seq[0][0];
  
  if(Fork()==0){
    execvp(commande,l->seq[0]);
  }
  else{
    Wait(NULL);
  }

}

		