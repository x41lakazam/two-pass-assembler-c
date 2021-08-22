/*
 * Error checking functions
 */

#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include "errors.h"
#include "globals.h"
#include "encoder.h"
#include "instructions.h"
#include "labels.h"

void raise_error(char* msg)
{
    if (msg != NULL)
        printf("%s\n", msg);
    printf("Exiting with code 1.\n");
	exit(1);
}

void check_file(char *fname, size_t line_len){
    FILE *fp;
    int error_raised, line_no;
	size_t read_cnt; /* Number of character retrieved on a line */
    char *line_ptr;
    char *label = (char *) calloc(LINE_MAX_SIZE, sizeof(char));
    char *cmd;

    fp = fopen(fname, "r");
    if (fp == NULL){
		printf("[x] Bad file: %s\n", fname);
        raise_error(NULL);
    }

    error_raised = line_no = 0;
    line_ptr = (char *) calloc(LINE_MAX_SIZE, sizeof(char));

	while ((read_cnt = get_line_wout_spaces(&line_ptr, &line_len, fp)) != -1) {

		cmd = (char *) calloc(CMD_MAX_SIZE, sizeof(char));
        ++line_no;

        if (!relevant_line(line_ptr))
            continue;

        /* Check several errors: */

        /* Error 1 - Line is too long */
        if (read_cnt >= LINE_MAX_SIZE){
            printf("[x] Error on line %d: line too long (%lu chars, maximum is 80)\n", line_no, read_cnt);
            error_raised = 1;
        }

        /* Error 2 - Line contains open quotes */
        if (open_quotes(line_ptr)){
            printf("[x] Error on line %d: Invalid syntax - <%s> (quote left open)\n", line_no, line_ptr);
            error_raised = 1;
        }

        /* Error 3 - The line contains a colon but no label */
        if (!validate_prefix(line_ptr)){
            printf("[x] Error on line %d: Invalid syntax - <%s> (label name is empty)\n", line_no, line_ptr);
			line_ptr++; /* Skip the colon to check more errors */
            error_raised = 1;
        }

        /* Error 4 - double commas */
        if (!validate_commas(line_ptr)){
            printf("[x] Error on line %d: Invalid syntax - <%s> (consecutive commas)\n", line_no, line_ptr);
            error_raised = 1;
        }

        /* Error 5 - If there is a label, check that it's not a forbidden word */
        if (contain_label(line_ptr)){
            label = get_label(line_ptr);
            if (!validate_label(label, line_no))
                error_raised = 1;

			line_ptr = trim_label(line_ptr);
        }

        /* If it's a code instruction */
        if (is_code_instruction(line_ptr)){
            get_cmd_name(line_ptr, cmd);
            /* Error 6 - Check that the command exists */
			if (!command_exists(cmd)){
                printf("[x] Error on line %d: Command <%s> doesn't exist.\n", line_no, cmd);
                error_raised = 1;
            }
			else{
				/* Error 7 - Check that the number of arguments match the command requirements */
				if (!check_number_of_args(line_ptr, line_no))
					error_raised = 1;

				/* Error 8 - Check that the registers name are right */
				if (!check_registers(line_ptr, line_no))
					error_raised=1;
			}
        }
    }
    /* If any error occured, the file cannot be parsed, stop execution */
    if (error_raised == 1)
        raise_error(NULL);

}

bool open_quotes(char* line_ptr)
{
	int quotes_counter = 0; /* count the qoutes in the line */

	char* current_char = line_ptr; /* initialize the current char pointer */

	char quote = '"';

	while (*current_char)
	{
		if (quote == *current_char) /* character is a quote */
			quotes_counter++;

		current_char++;
	}

	/* if the number of quotes in the line is even then all quotes are closed */
	if (quotes_counter % 2 != 0)
		return true;
	return false;
}

bool validate_register(int reg_num)
{
	if (reg_num >= 0 && reg_num <= 32) /* as written in the project instructions */
		return true;
	return false;
}

bool validate_prefix(char* line_ptr)
{
	if (*line_ptr == ':')
		return false;
	return true;
}

bool check_number_of_args(char* line_ptr, int line_no){
	char* cmd; /* the command name */
	char* args; /* the command arguments */
	int cmd_opcode;
	int required_args;
	int args_counter = 0; /* count the given argumetns */
	char* token; /* for strtok and counting the args */

	cmd = (char *) calloc(80, sizeof(char));
	get_cmd_name(line_ptr, cmd);

	args = (char *) calloc(80, sizeof(char));
	strcpy(args, line_ptr + strlen(cmd));

	cmd_opcode = get_opcode(cmd);

	token = strtok(args, ",");
	while (token != NULL)
	{
		args_counter++;
		token = strtok(NULL, ",");
	}

	if (cmd_opcode == 0) /* R instructions - 3 operands */
		required_args = 3;
	else if (cmd_opcode == 1) /* R instructions - 2 operands */
		required_args = 2;
	else if (cmd_opcode >= 10 && cmd_opcode <= 24) /* I instrcution */
		required_args = 3;
	else if (cmd_opcode >= 30 && cmd_opcode <= 62) /* J instrcution */
		required_args = 1;
	else if (cmd_opcode == 63)
		required_args = 0;

	if (args_counter != required_args){
		printf("[x] Error on line %d: Bad number of parameters (Actual: %d, expected: %d)\n", line_no, args_counter, required_args);
		return false;
	}

	return true;
}

bool validate_commas(char* line_ptr)
{
	char* cmd; /* the command name */
	char* args; /* the command arguments */
	int comma_found = 0;
	char* current_char;

	cmd = (char *) calloc(80, sizeof(char));
	get_cmd_name(line_ptr, cmd);

	args = (char *) calloc(80, sizeof(char));
	strcpy(args, line_ptr + strlen(cmd));
	current_char = args;

	/* Iterate over every character and check for doubled commas */
	while (*current_char)
	{
		if (*current_char == ',')
		{
			/* If a comma was already found, then commas are consecutive */
			if (comma_found == 1)
				return false;

			comma_found = 1; /* a comma is found */
		}
		/* Any non-space character cancel the comma flag */
		else if ( !isspace(*current_char))
			comma_found = 0;

		current_char++;
	}

	return true;
}

bool is_reserved_word(char* word)
{
	if (get_opcode(word) != -1) /* if there's an opcode for the word, it's a reserved one */
		return true;
	return false;
}

bool command_exists(char *cmd_name){
    if (get_opcode(cmd_name) == -1){
        return false;
    }
    return true;
}

bool validate_label(char *lbl_name, int line_no){
    int i = 0;

	/* Check that the label contain only alphanumeric characters */
	while (lbl_name[i]){
		if ( !isalnum(lbl_name[i++]) ){
            printf("[x] Error on line %d: Bad Label <%s>, labels can contain only alphanumeric characters\n", line_no, lbl_name);
			return false;
        }
	}

    /* Check if the label is a reserved word */
    if (is_reserved_word(lbl_name)){
        printf("[x] Error on line %d: Bad Label, <%s> is a reserved word\n", line_no, lbl_name);
        return false;
    }

	return true;
}

bool check_registers(char *line_ptr, int line_no){
    int val;
    char *token;
    char *params;

	params = strchr(line_ptr, ' ');
	if(params != NULL)
	{
		params = params + 1;
	}

    token = strtok(params, ",");

    while (token){
        if (*token == '$'){
            if (*(token+1) == '0')
                val = 0;
            else{
                val = atoi(token+1);
                if (val == 0){
                    printf("[x] Error on line %d: Register %s is invalid (it should be a number between 0 and 31)\n", line_no, token);
                    return false;
                }
            }

            if ( val < 0 || val > 31){
                printf("[x] Error on line %d: Register %s is not in range (should be between 0 and 31)\n", line_no, token);
                return false;
            }
        }
		token = strtok(NULL, ",");
    }
    return true;
}
