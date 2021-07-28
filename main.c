#include <stdlib.h>
#include <stdio.h>
#include "globals.h"

#include "first_pass.c"
#include "second_pass.c"

void assemble_file(char* file_name)
{
    FILE *fp;

	fp = fopen(file_name, "r");
	if (fp == NULL)
	{
		printf("error while opening file");
		exit(0);
	}

    first_pass(fp);

	fclose(fp);
}

int main(int argc, char* argv[])
{
	int i;
	if (argc <= 1){
		printf("no file passed");
        exit(0);
    }

    for (i = 1; i < argc; i++)
        assemble_file(argv[i]);

	return 0;
}
