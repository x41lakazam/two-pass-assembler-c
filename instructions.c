/*
 * Instructions related functions
 */

#include <stdbool.h>
#include "utils.h"

#define MAX_CMD_LENGTH 80

/*
 * Instructions table
 * Store every possible instruction
 */
char *instructions[] = {
    ".db",
    ".dh",
    ".dw",
    ".asciz"
};

/*
 * Store the number of available instructions
 */
int instructions_cnt = 4;

/*
 * Return True if a string is a valid instruction
 */
bool is_instruction(char *s){
    int i;
    /*
     * Not an instruction if it doesn't start
     * with a point
     */
    if (*s != '.')
        return false;

    for (i=0; i < instructions_cnt; i++){
        if (starts_with(s, instructions[i]))
            return true;
    }
    return false;
}

char *get_instruction(char *line_ptr){
	char* find_instruction; 
	char* name;
	int i;
	char* line_cpy =  (char *) calloc(MAX_CMD_LENGTH, sizeof(char));
	strcpy(line_cpy, line_ptr); 
	for (i=0; i < instructions_cnt; i++)
	{
		find_instruction = strstr(line_cpy, instructions[i]);
		if (find_instruction)
		{
			name = strtok(find_instruction, " ");
			return name;
		}
	}
	return NULL; 
}

bool is_entry_instruction(char *line_ptr) {
	char* entry = ".entry"; 
	if (strstr(line_ptr, entry))
	{
		return true; 
	}
	return false; 
}

bool is_external_instruction(char *line_ptr) {
	char* external = ".extern"; 
	if (strstr(line_ptr, external))
	{
		return true; 
	}
	return false; 
}

char *parse_external_var_name(char *line_ptr) {
	/* i am assuming that the var name is always the second word in the line */

	char* token;
	char* line_cpy =  (char *) calloc(80, sizeof(char));
	strcpy(line_cpy, line_ptr);
	token = strtok(line_cpy, " ");  
	token = strtok(NULL, " ");
	return token; 
}




