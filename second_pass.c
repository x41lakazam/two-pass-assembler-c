#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

void second_pass(char *fname, struct Node *external_labels_ptr){
	FILE *fp;
	fp = fopen(fname, "r");

	fclose(fp);
}