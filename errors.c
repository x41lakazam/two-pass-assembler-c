#include <stdlib.h>
#include "errors.h"
#include "globals.h"

void raise_error(char* msg)
{
    if (msg != NULL)
        printf("%s\n", msg);
    printf("Exiting with code 1.");
	exit(1);
}

bool validate_cmd_length(char* line_ptr)
{
	if (strlen(line_ptr) <= LINE_MAX_SIZE)
		return true;

	return false;
}

bool open_quotes(char* line_ptr)
{
	int quotes_counter = 0; /* count the qoutes in the line */

	char* currentChar = line_ptr; /* initialize the current char pointer */

	char quote = '"';

	while (*currentChar)
	{
		if (quote == *currentChar) /* a qoute is found */
		{
			quotes_counter++;
		}
	}

	/* if the number of quotes in the line is even then all quotes are closed */
	if (quotes_counter % 2 != 0)
	{
		return true;
	}
	return false;
}

bool is_register_exists(int reg_num)
{
	if (reg_num >= 0 && reg_num <= 32) /* as written in the project instructions */
	{
		return true;
	}
	return false;
}

bool colon_without_label(char* line_ptr)
{
	char* colon_ptr = strchr(line_ptr, ':'); /* pointer to the first occurence of ':' in the line */

	if (colon_ptr == line_ptr) /* the first character is ':' */
	{
		return true;
	}

	return false;
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
	{
		return true; 
	}
	else if (cmd_opcode == 1 && args_counter == 2)/* R instructions - 2 operands */
	{
		return true; 
	}
	else if (cmd_opcode >= 10 && cmd_opcode <= 24 && args_counter == 3) /* I instrcution */
	{
		return true; 
	}
	else if (cmd_opcode >= 30 && cmd_opcode <= 63 && args_counter == 1) /* J instrcution */
	{
		return true; 
	}
	return false; 
}


bool is_value_in_range(int value, int start, int end)
{
	if (value >= start && value <= end)
	{
		return true;
	}
	return false;
}

bool validate_commas(char* line_ptr)
{
	char* cmd; /* the command name */ 
	char* args; /* the command arguments */ 
	char* token; /* to parse the arguemnts */
	int comma_found = 0;
	char* currentChar; 

	cmd = (char *) calloc(80, sizeof(char));
	get_cmd_name(line_ptr, cmd);

	args = (char *) calloc(80, sizeof(char));
	strcpy(args, line_ptr + strlen(cmd)); 
	currentChar = args; 
	
	if (strchr(cmd, ',')) /* comma in command name is illeagl */
	{
		return false; 
	} 

	while (*currentChar)
	{
		if (*currentChar == ',')
		{
			if (comma_found == 1)
			{
				return false; /* consecutive commas */
			}
			comma_found = 1; /* a comma is found */
		}
		else if (*currentChar != ' ')
		{
			comma_found = 0;
		}
		*currentChar++; 
	}
	
	return true;
}


bool is_reserved_word(char* word)
{
	if (get_opcode(word) != -1) /* if there's an opcode for the word, it's a reserved one */ 
	{
		return true; 
	}
	return false;
}

