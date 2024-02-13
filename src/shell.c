/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "utils.h"

int main()
{
	// Variable to set the prompt
	char *path;
	char *user = getenv("USER");
	
	while (1) {
		
		// Remember the path of thhe current directory
		path = getcwd(NULL, 0);
		struct cmdline *l;

		printf("%s:%s$ ", user, path);
		l = readcmd();

		char *commande = l->seq[0][0];

		/* If input stream closed, normal termination */
		if (!l) { // Ctrl+d
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		if(is_internal(commande)){
			exec_internal(l);
		}
		else{
			exec_external(l);
		}

	}

}
