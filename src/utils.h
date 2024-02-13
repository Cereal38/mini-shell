#ifndef __UTILS_H
#define __UTILS_H

#include "readcmd.h"

int is_internal (char *cmd);
void exec_internal (struct cmdline *l);
void exec_external (struct cmdline *l);

#endif