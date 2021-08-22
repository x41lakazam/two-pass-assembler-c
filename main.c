/*
 * Assemble assembler code (.as file)
 * The assembling is done in two passes:
 * The first pass mostly calculate the address of each label and store them into a table.
 * The second pass encode every line and dump it to a file.
 *
 * Temporary files are used during the second pass, and will be automatically deleted
 * at the end of the assembling.
 */
#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "first_pass.h"

int main(int argc, char* argv[])
{
	int i;

	if (argc <= 1){
		printf("no file passed\n");
        exit(0);
    }

    /* Process every file */
    for (i = 1; i < argc; i++){
        printf("Processing file %s\n", argv[i]);
        first_pass(argv[i]);
    }

    printf("Assembling finished.\n");

	return 0;
}
