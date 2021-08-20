#include "errors.h"

void print_msg(int line_num, char* msg) 
{
	printf("error in line %d: %s\n", line_num, msg);
	exit(1);
}

bool validate_cmd_length(char* line_ptr)
{
	if (strlen(line_ptr) <= MAX_CMD_LENGTH)
	{	
		return true; 
	}
	return false;
}

bool is_command_exists(char* cmd)
{
	
}

bool open_qoutes(char* line_ptr)
{
	int quotes_counter = 0; /* count the qoutes in the line */ 

	char* currentChar = line_cpy; /* initialize the current char pointer */ 

	char quote = '"';

	while (*currentChar)
	{
		if (quote == *currentChar) /* a qoute is found */
		{
			quotes_counter++;
		}
	}
	
	/* if the number of qoutes in the line is even then all qoutes are closed */
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

bool check_number_of_args(char* cmd, char* args)
{

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
}

