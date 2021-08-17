#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "linked_list.h"

#include "linked_list.h"
#include "first_pass.h"
#include "second_pass.h"
#include "linked_list.h"

/* For now, compile with gcc main.c */

int main(int argc, char* argv[])
{
	int i;
	char *of = "a.out"; /* TODO */
	if (argc <= 1){
		printf("no file passed");
        exit(0);
    }

    /* Make first pass on every file */
    for (i = 1; i < argc; i++)
        first_pass(argv[i], of);

    /* Make second pass on every file */
    /* for (i = 1; i < argc; i++) */
    /*     second_pass(argv[i], external_labels_ptr); */

	return 0;
}
