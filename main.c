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
#include <stdbool.h>
#include "globals.h"
#include "first_pass.h"
#include "errors.h"

int main(int argc, char* argv[])
{
	int i;
    bool is_valid;

    is_valid = true;

	if (argc <= 1){
		printf("no file passed\n");
        exit(0);
    }


    printf("Checking errors.\n");
    for (i = 1; i < argc; i++){
        printf("[*] Checking file %s\n", argv[i]);
        if (!check_file(argv[i]))
            is_valid = false;
    }

    if (!is_valid){
        printf("Errors in files, exiting.\n");
        exit(1);
    }

    /* Process every file */
    for (i = 1; i < argc; i++){
        printf("[*] Processing file %s\n", argv[i]);
        first_pass(argv[i]);
    }

    printf("[v] Assembling finished without errors.\n");

	return 0;
}
