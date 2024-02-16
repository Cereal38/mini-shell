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
  // pipeFd[1] = STDIN
  // pipeFd[0] = STDOUT
  // Children with pipe
  if(l->seq[1]){
    char *commande2 = l->seq[1][0];
    
    int p[2];
    pipe(p);
    pid_t childPid1;
    if ((childPid1 = Fork()) == 0) { // fils
            Close(p[0]); 
            
            redirectInOut(l);
            Dup2(p[1], STDOUT);
            execvp(commande,l->seq[0]);
            gestion_erreur(commande);
            
            Close(p[1]); 
            exit(0);
        } else { // père

            pid_t childPid2 ;
            if ((childPid2 = Fork())== 0) //fils 2
            {
                Close(p[1]); 
                Dup2(p[0], STDIN);
                redirectInOut(l);
                execvp(commande2,l->seq[1]);
                gestion_erreur(commande2);

                Close(p[0]);
                exit(0);
            }
            Close(p[0]);
            Close(p[1]);
            while (wait(NULL) > 0);
  }}
  
  // No pipe
  else if(Fork()==0){
    // Redirect input and output
    redirectInOut(l);
    execvp(commande,l->seq[0]);
    gestion_erreur(commande);
    exit(0);
  } else {
    Wait(NULL);
  }
}
		