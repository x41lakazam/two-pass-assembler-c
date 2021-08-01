#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "linked_list.h"

#include "linked_list.c"
#include "first_pass.c"
#include "second_pass.c"

/* For now, compile with gcc main.c */

int main(int argc, char* argv[])
{
	int i;
    struct Node *external_labels_ptr;
	if (argc <= 1){
		printf("no file passed");
        exit(0);
    }


    /* Create external labels table */
    external_labels_ptr = node_create(-1, -1, -1);

    /* Make first pass on every file */
    for (i = 1; i < argc; i++)
        first_pass(argv[i], external_labels_ptr);

    /* Make second pass on every file */
    /* for (i = 1; i < argc; i++) */
    /*     second_pass(argv[i], external_labels_ptr); */


	return 0;
}
