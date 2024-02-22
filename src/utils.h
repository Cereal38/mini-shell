#ifndef __UTILS_H
#define __UTILS_H

#include "readcmd.h"

// File descriptors
#define STDIN 0
#define STDOUT 1
#define STDERR 2

// Colors
#define BLUE "\033[0;34m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

#define MAX_SIZE_PROC_ARRAY 1000

#define BG 1
#define FG 0

/*
Add a process to the foreground / background array of process

Input:
  pid: PID to ad to the array
  type: 0 to add to the background array
        1 to add to the background array
*/
void add_processus(pid_t pid, int type);

/*
Remove a process from the foreground / background array of process

Input:
  pid: PID to remove from the array
  type: 0 to remove from the background array
        1 to remove from the background array
*/
void remove_processus(pid_t pid, int type);

/*
Handler for the SIGCHLD signal
It waits for the child process and removes it from the foreground / background array

Input:
  sig: int
*/
void handler_child(int sig);

/*
Handler for the SIGINT signal
It "deactivates" the SIGINT (Ctrl+C) signal. It is used to prevent the shell from being interrupted

Input:
  sig: int
*/
void handler_interrupt_shell(int sig);

int is_background(pid_t pid);

/*
Return true if the command is internal

Input:
  cmd: string (Command to analyze)

Output:
  1 if the command is an internal command
  0 if not
*/
int is_internal(char *cmd);

/*
    Exec an internal command

    Input:
      l: cmdline* (Current cmdline)
*/
void exec_internal(struct cmdline *l);

/*
    Exec an external command in a child process and wait for it to finish in the parent process

    Input:
      l: cmdline* (Current cmdline)
*/
void exec_external(struct cmdline *l);

/*
    Redirect input if needed

    Input:
      l: cmdline* (Current cmdline)
      in: int* (File descriptor for input redirection)

    Output:
      1 if the redirection was successful
      0 if not
*/
int handle_input_redirection(struct cmdline *l, int *in);

/*
    Redirect output if needed

    Input:
      l: cmdline* (Current cmdline)
      in: int* (File descriptor for output redirection)

    Output:
      1 if the redirection was successful
      0 if not
*/
int handle_output_redirection(struct cmdline *l);

/*
    Print an error message to stderr depending on the error code

    Input:
      command: string (Command that caused the error)
*/
void error_handling(char *command);
#endif