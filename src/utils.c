#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "utils.h"


void gestion_erreur(char *commande){
  if (errno == ENOENT) {
    fprintf(stderr, "%s: Command not found\n", commande);
  }
  else if (errno == EACCES) {
    fprintf(stderr, "%s: Permission denied\n", commande);
  }
  else {
    fprintf(stderr, "%s: Error %s\n", commande, strerror(errno));
  }
}

void redirectInOut (struct cmdline *l) {
  if (l->in) {
    int fd = Open(l->in, O_RDONLY, 0);
    Dup2(fd,STDIN);
  }
  if (l->out) {
    int fd = Open(l->out, O_WRONLY | O_CREAT, 0);
    Dup2(fd, STDOUT);
  }
  
}


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
    // Redirect input and output
    redirectInOut(l);
    execvp(commande,l->seq[0]);
    gestion_erreur(commande);
    exit(0);
  } else {
    Wait(NULL);
  }

}


		