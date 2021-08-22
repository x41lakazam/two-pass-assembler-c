/*
 * Instructions related functions (parsers, checkers..)
 * - Parse instructions out of a string
 * - Check if an instruction is of a certain type
 * - Retrieve additional information out of instructions (for example: opcode)
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "globals.h"
#include "errors.h"
#include "instructions.h"

#define MAX_CMD_LENGTH 80

/* Create arrays that contain the commands by groups */
int R_cmds_len = 8;
char R_cmds[8][5] = {
	"add",
	"sub",
	"and",
	"or",
	"nor",
	"move",
	"mvhi",
	"mvlo"
};

int I_cmds_len = 15;
char I_cmds[15][5] = {
	"addi",
	"subi",
	"andi",
	"ori",
	"nori",
	"bne",
	"beq",
	"blt",
	"bgt",
	"lb",
	"sb",
	"lw",
	"sw",
	"lh",
	"sh"
};

int J_cmds_len = 4;
char J_cmds[4][5] = {
	"jmp",
	"la",
	"call",
	"stop"
};

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
bool is_data_instruction(char *s){
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
	char* find_instruction; /* pointer to the begiining of the instruction in the line */
	char* name;
	int i;

	char* line_cpy =  (char *) calloc(MAX_CMD_LENGTH, sizeof(char)); /* strtok changes the string */
	strcpy(line_cpy, line_ptr);

	/* look for every possible instruction */
	for (i=0; i < instructions_cnt; i++)
	{
		find_instruction = strstr(line_cpy, instructions[i]);
		if (find_instruction)
		{
			name = strtok(find_instruction, " "); /* parse instruction name */
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
		return true;

	return false;
}

char *parse_external_var_name(char *line_ptr) {
	/* i am assuming that the var name is always the second word in the line */

	char* token; /* for strtok */

	char* line_cpy =  (char *) calloc(80, sizeof(char)); /* strtok changes the original string */
	strcpy(line_cpy, line_ptr);

	token = strtok(line_cpy, " ");
	token = strtok(NULL, " ");
	return token;
}

int get_required_cells(char *line_ptr) {
	char* instruction_name = get_instruction(line_ptr);
	char* token; /* for strtok - parse the instruction parameters */
	int counter = 0; /* counts the required cells ans retunrs them */

	/* parse out the instrcuctions parameters without the instruction name */
	char* instruction_params = (char *) calloc(80, sizeof(char));
	strcpy(instruction_params, line_ptr + strlen(instruction_name));
	instruction_params = trim_whitespaces(instruction_params);

	if (STREQ(instruction_name, ".asciz"))
	{
		counter += strlen(instruction_params) - 2;/* not counting the double quotes */
        counter += 1; /* Add the \0 char */
	}
	else
	{
		token = strtok(instruction_params, ",");
		while (token)
		{
			if (strcmp(instruction_name, ".db") == 0)
			{
				counter += 1;
			}
			else if (strcmp(instruction_name, ".dh") == 0)
			{
				counter += 2;
			}
			else if (strcmp(instruction_name, ".dw") == 0)
			{
				counter += 4;
			}
			token = strtok(NULL, ",");
		}
	}
	return counter;
}

bool relevant_line(char *s){

    s = trim_whitespaces(s);

    if (*s == '\n')
        return false;

    if (*s == COMMENT_CHAR)
        return false;

    return true;
}

void get_cmd_name(char *line_ptr, char *buf){
    int i = 0;

    while (i < CMD_MAX_SIZE && *line_ptr != ' '){
        *buf++ = *line_ptr++;
        i++;
    }
}

InstructionsGroup get_instruction_group(char *cmd_name){
    /* Assuming cmd_name is a valid command name */
    int i;

    /* Instruction is in R group ? */
    for (i=0; i<R_cmds_len; i++)
        if (STREQ(R_cmds[i], cmd_name)) return R;

    /* Instruction is in I group ? */
    for (i=0; i<I_cmds_len; i++)
        if (STREQ(I_cmds[i], cmd_name)) return I;

    /* Else instruction is in J group */
    return J;
}

int get_opcode(char *cmd_name){
    if (STREQ(cmd_name, "add") ||
        STREQ(cmd_name, "sub") ||
        STREQ(cmd_name, "and") ||
        STREQ(cmd_name, "or")  ||
        STREQ(cmd_name, "nor")) return 0;
    else if (STREQ(cmd_name, "move") ||
             STREQ(cmd_name, "mvhi") ||
             STREQ(cmd_name, "mvlo")) return 1;
    else if (STREQ(cmd_name, "addi")) return 10;
    else if (STREQ(cmd_name, "subi")) return 11;
    else if (STREQ(cmd_name, "andi")) return 12;
    else if (STREQ(cmd_name, "ori")) return 13;
    else if (STREQ(cmd_name, "nori")) return 14;
    else if (STREQ(cmd_name, "bne")) return 15;
    else if (STREQ(cmd_name, "beq")) return 16;
    else if (STREQ(cmd_name, "blt")) return 17;
    else if (STREQ(cmd_name, "bgt")) return 18;
    else if (STREQ(cmd_name, "lb")) return 19;
    else if (STREQ(cmd_name, "sb")) return 20;
    else if (STREQ(cmd_name, "lw")) return 21;
    else if (STREQ(cmd_name, "sw")) return 22;
    else if (STREQ(cmd_name, "lh")) return 23;
    else if (STREQ(cmd_name, "sh")) return 24;
    else if (STREQ(cmd_name, "jmp")) return 30;
    else if (STREQ(cmd_name, "la")) return 31;
    else if (STREQ(cmd_name, "call")) return 32;
    else if (STREQ(cmd_name, "stop")) return 63;

    return -1;
}

int get_function_id(char *cmd_name){
    if (STREQ(cmd_name, "add")) return 1;
    else if (STREQ(cmd_name, "sub")) return 2;
    else if (STREQ(cmd_name, "and")) return 3;
    else if (STREQ(cmd_name, "or")) return 4;
    else if (STREQ(cmd_name, "nor")) return 5;
    else if (STREQ(cmd_name, "move")) return 1;
    else if (STREQ(cmd_name, "mvhi")) return 2;
    else if (STREQ(cmd_name, "mvlo")) return 3;

    return -1;
}

bool is_code_instruction(char *line_ptr){
	if (relevant_line(line_ptr) &&
		!is_data_instruction(line_ptr) &&
		!is_entry_instruction(line_ptr) &&
		!is_external_instruction(line_ptr)
	   )
		return true;
	return false;
}

