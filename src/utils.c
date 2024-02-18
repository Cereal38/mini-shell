#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "utils.h"

void gestion_erreur(char *commande)
{
  if (errno == ENOENT)
  {
    fprintf(stderr, "%s: Command not found\n", commande);
  }
  else if (errno == EACCES)
  {
    fprintf(stderr, "%s: Permission denied\n", commande);
  }
  else
  {
    fprintf(stderr, "%s: Error %s\n", commande, strerror(errno));
  }
}

void redirectInOut(struct cmdline *l)
{
  if (l->in)
  {
    int fd = Open(l->in, O_RDONLY, 0);
    Dup2(fd, STDIN);
  }
  if (l->out)
  {
    int fd = Open(l->out, O_WRONLY | O_CREAT, 0);
    Dup2(fd, STDOUT);
  }
}

int is_internal(char *cmd)
{
  if (strcmp(cmd, "quit") == 0)
  {
    return 1;
  }
  else if (strcmp(cmd, "cd") == 0)
  {
    return 1;
  }

  return 0;
}

void exec_internal(struct cmdline *l)
{

  if (strcmp(l->seq[0][0], "quit") == 0)
  {
    exit(0);
  }

  else if (strcmp(l->seq[0][0], "cd") == 0)
  {
    if (chdir(l->seq[0][1]) != 0)
    {
      printf("Error: %s\n", strerror(errno));
    }
  }
}

void exec_external(struct cmdline *l)
{
  int p[2], in = 0; // 'in' is the input file descriptor for the next command
  pid_t pid;

  // Iterate over each command in the pipeline
  for (int i = 0; l->seq[i] != NULL; i++)
  {
    pipe(p); // Create a new pipe

    if ((pid = fork()) == 0)
    { // Child process
      if (in != 0)
      {                         // If 'in' is not standard input
        dup2(in, STDIN_FILENO); // Redirect 'in' to standard input
        close(in);              // Close the used file descriptor
      }

      if (l->seq[i + 1] != NULL)
      {                            // If there is a next command
        dup2(p[1], STDOUT_FILENO); // Redirect standard output to 'p[1]'
      }

      close(p[0]); // Close the unused read end of the pipe

      execvp(l->seq[i][0], l->seq[i]);
      gestion_erreur(l->seq[i][0]);
      exit(EXIT_FAILURE);
    }
    else
    {             // Parent process
      wait(NULL); // Wait for the child process to finish

      if (in != 0)
      {            // If 'in' is not standard input
        close(in); // Close the used file descriptor
      }

      close(p[1]); // Close the unused write end of the pipe
      in = p[0];   // Save the read end of the pipe as 'in' for the next command
    }
  }
}
