#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_CMD_LENGTH 80;

/*
This method prints a proper error message.
Args:
line_num - the number of the line where the error occured.
msg - the message to display.
*/
void print_msg(int line_num, char* msg); 

/*
This method checks the length of the command to make sure it's not too long -
(more than 80 characters).
Args:
line_ptr - the line to check.
Return:
true if the line is in the allowed range of length and false if not. 
*/
bool validate_cmd_length(char* line_ptr); 

/*
This method checks if a specific command exists or not.
Args:
cmd - the command name to check.
Return:
true if the command exists and false if not. 
*/
bool is_command_exists(char* cmd);

/*
This method checks if a specific register exists or not.
Args:
reg_num - the register number to check.
Return:
true if the register number is valid and false if not. 
*/
bool is_register_exists(int reg_num);

/*
This method checks if there are open qoutes in the line.
It counts the total number of qoutes and checks if it's even.
Args:
line_ptr - the line to check.
Return:
true if open qoutes are found and false if not. 
*/
bool open_qoutes(char* line_ptr);

/*
This method checks a there is a colon (:) without a label in the line. 
It checks if the first character in the line is a colon.
Args:
line_ptr - the line to check.
Return:
true if a colon without a label is found and false if not. 
*/
bool colon_without_label(char* line_ptr);

/*
This method checks if the command has the proper number of arguments.  
Args:
cmd - the command name.
args - the command arguments. 
Return:
true if the given number of args is valid and false if not. 
*/
bool check_number_of_args(char* cmd, char* args);

/*
This method checks a specific value is within a given range.
Args:
value - the value to check.
start - the beginning of the range.
end - the end of the range.
Return:
true if the value is within the range and false if not. 
*/
bool is_value_in_range(int value, int start, int end); 

/*
This method checks all comma-realted errors: commas at beginning or end, 
or consecutive commas.
Args:
line_ptr - the line to validate.
Return:
true if there are no comma-related error and false if not.
*/
bool validate_commas(char* line_ptr);














