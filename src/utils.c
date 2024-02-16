#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "utils.h"

int is_internal (char *cmd) {  
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
    // char *in;	/* If not null : name of file for input redirection. */
	 // char *out;	/* If not null : name of file for output redirection. */
    if (l->in) {
      int fileIn = Open(l->in, O_RDONLY, 0);
      dup2(fileIn, STDIN);
    }
    if (l->out) {
      int fileOut = Open(l->out, O_CREAT | O_WRONLY, 0);
      dup2( fileOut, STDOUT);
    }
    execvp(commande,l->seq[0]);
  } else {
    Wait(NULL);
  }

}


		