/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"


int main()
{
	while (1) {
		struct cmdline *l;
		int i, j;

		printf("shell> ");
		l = readcmd();

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		if (strcmp(l->seq[0][0], "quit") == 0)
		{
			exit(0);
		}
		// All external commands are processed here
		else {
			// We create a child to run the external cmd
			if (Fork() == 0) { // Child
				execvp(l->seq[0][0],l->seq[0][1]);
			}

			else { // Parent
				Wait(NULL);
			}
		}
		
		
	}


}
