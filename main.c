#include <stdlib.h>
#include <stdio.h>
#include "globals.h"

#include "first_pass.h"

/* For now, compile with gcc main.c */

int main(int argc, char* argv[])
{
	int i;

	if (argc <= 1){
		printf("no file passed");
        exit(0);
    }

    /* Make first pass on every file */
    for (i = 1; i < argc; i++)
        first_pass(argv[i]);

	return 0;
}
