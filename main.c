#include "assembler.h"

int main(int argc, char* argv[])
{
	int i;
	if (argc <= 1)
	{
		printf("no file passed");
	}
	else
	{
		for (i = 1; i < argc; i++)
		{
			assemble_file(argv[i]); 
		}
	}
	return 0; 
}

void assemble_file(char* file_name)
{
	file_content = fopen(file_name, "r"); 
	if (!file_content)
	{
		printf("error while opening file");
		exit(0);
	}
	else
	{
		first_pass(file_content); 
	}
	fclose(file_content);
}




