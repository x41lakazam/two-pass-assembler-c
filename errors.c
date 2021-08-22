#include <stdlib.h>
#include <ctype.h>
#include "errors.h"
#include "globals.h"
#include "encoder.h"
#include "instructions.h"

void raise_error(char* msg)
{
    if (msg != NULL)
        printf("%s\n", msg);
    printf("Exiting with code 1.\n");
	exit(1);
}

void check_file(char *fname, size_t line_len){
    FILE *fp;
    int error_raised;
	ssize_t read_cnt; /* Number of character retrieved on a line */
    char *line_ptr;

    fp = fopen(fname, "r");
    if (fp == NULL){
		printf("Bad file: %s\n", fname);
        raise_error(NULL);
    }

    line_ptr = (char *) calloc(LINE_MAX_SIZE, sizeof(char));

	while ((read_cnt = getline(&line_ptr, &line_len, fp)) != -1) {
        /* Check several errors: */

        /* Error 1 - Line is too long */
        if (read_cnt >= LINE_MAX_SIZE){
            printf("Line is too long (%zd chars, maximum is 80)\n", read_cnt);
            error_raised = 1;
        }

        /* Error 2 - Line contains open quotes */
        if (open_quotes(line_ptr)){
            printf("Invalid syntax on line: <%s> (quote left open)\n", line_ptr);
            error_raised = 1;
        }

        /* Error 3 - The line contains a colon but no label */
        if (!validate_prefix(line_ptr)){
            printf("Invalid syntax on line: <%s> (label name is empty)\n", line_ptr);
            error_raised = 1;
        }

        /* Error 4 - double commas */
        if (!validate_commas(line_ptr)){
            printf("Invalid syntax on line: <%s> (doubled comma)\n", line_ptr);
            error_raised = 1;
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

bool check_number_of_args(char* line_ptr)
{
	char* cmd; /* the command name */
	char* args; /* the command arguments */
	int cmd_opcode;
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

	if (cmd_opcode == 0 && args_counter == 3) /* R instructions - 3 operands */
		return true;
	else if (cmd_opcode == 1 && args_counter == 2)/* R instructions - 2 operands */
		return true;
	else if (cmd_opcode >= 10 && cmd_opcode <= 24 && args_counter == 3) /* I instrcution */
		return true;
	else if (cmd_opcode >= 30 && cmd_opcode <= 63 && args_counter == 1) /* J instrcution */
		return true;
	return false;
}


bool is_value_in_range(int value, int start, int end)
{
	if (value >= start && value <= end)
		return true;
	return false;
}

bool validate_commas(char* line_ptr)
{
	char* cmd; /* the command name */
	char* args; /* the command arguments */
	char* token; /* to parse the arguemnts */
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

bool validate_label(char *lbl_name){
	/* Check that the label contain only alphanumeric characters */
	while (*lbl_name){
		if ( !isalpha(*lbl_name++) )
			return false;
	}
	return true;
}

