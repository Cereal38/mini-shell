#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "utils.h"

pid_t proc_foreground[MAX_SIZE_PROC_ARRAY] = {-1};
pid_t proc_background[MAX_SIZE_PROC_ARRAY] = {-1};

void add_processus(pid_t pid , int type) {
  if(type==FG){
    int i = 0;
    while (proc_foreground[i] != -1) {
      i++;
    }
    proc_foreground[i] = pid;
    proc_foreground[i + 1] = -1;
  }
  else{
    int i = 0;
    while (proc_background[i] != -1) {
      i++;
    }
    proc_background[i] = pid;
    proc_background[i + 1] = -1;
  }
}

void remove_processus(pid_t pid , int type) {
  if(type==FG){
    int i = 0;
    while (proc_foreground[i] != pid) {
      i++;
    }
    proc_foreground[i] = -1;
  }
  else{
    int i = 0;
    while (proc_background[i] != pid) {
      i++;
    }
    proc_background[i] = -1;
  }
}

void error_handling(char *command)
{
  if (errno == ENOENT)
  {
    fprintf(stderr, "%s: Command not found\n", command);
  }
  else if (errno == EACCES)
  {
    fprintf(stderr, "%s: Permission denied\n", command);
  }
  else
  {
    fprintf(stderr, "%s: Error %s\n", command, strerror(errno));
  }
}

int handle_input_redirection(struct cmdline *l, int *in)
{
  int fd_in;
  if (l->in)
  {
    fd_in = open(l->in, O_RDONLY);
    if (fd_in == -1)
    {
      perror("open");
      return 0;
    }
    *in = fd_in; // Redirect standard input to the file
  }
  return 1;
}

int handle_output_redirection(struct cmdline *l)
{
  int fd_out;
  if (l->out)
  {
    fd_out = open(l->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1)
    {
      perror("open");
      return 0;
    }
    dup2(fd_out, STDOUT_FILENO); // Redirect standard output to the file
    close(fd_out);               // Close the file descriptor
  }
  return 1;
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

  if(l->is_background){
			printf("%d\n",l->is_background);
		}	
  if (strcmp(l->seq[0][0], "quit") == 0 || strcmp(l->seq[0][0], "exit") == 0)
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
  int in = 0; // 'in' is the input file descriptor for the next command
  pid_t pid;
  if(l->is_background){
			printf("Background process\n");
		}	
  // Redirect input if needed (wc < file.txt)
  if (!handle_input_redirection(l, &in))
  {
    return;
  }

  // Iterate over each command in the pipeline
  for (int i = 0; l->seq[i] != NULL; i++)
  {
    int p[2];
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

      // Redirect output if needed (ls > file.txt)
      else
      {
        if (!handle_output_redirection(l))
        {
          return;
        }
      }

      close(p[0]); // Close the unused read end of the pipe

      execvp(l->seq[i][0], l->seq[i]);
      error_handling(l->seq[i][0]);
      exit(EXIT_FAILURE);
    }
    else
    {             // Parent process
      add_processus(pid,l->is_background);
      wait(NULL); // Wait for the child process to finish

      if (in != 0)
      {            // If 'in' is not standard input
        close(in); // Close the used file descriptor
      }

      close(p[1]); // Close the unused write end of the pipe
      in = p[0];   // Save the read end of the pipe as 'in' for the next command
    }
  }
  int i = 0;
  while(proc_foreground[i] != -1){
    wait(NULL);
    remove_processus(pid,FG);
  }
}