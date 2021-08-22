#ifndef ERRORS_H
#define ERRORS_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
This method prints a proper error message.
Args:
msg - the message to display.
*/
void raise_error(char* msg);

/*
 * Check if any syntax error appear in a file
 * Args:
 * fname - Name of the file to check
 * line_len - Maximum length of a line
 */
void check_file(char *fname, size_t line_len);

/*
This method checks if a specific register exists or not.
Args:
reg_num - the register number to check.
Return: True if the register number is valid and false if not.
*/
bool validate_register(int reg_num);

/*
This method checks if there are open quotes in the line.
It counts the total number of quotes and checks if it's even.
Args:
line_ptr - the line to check.
Return:
true if open quotes are found and false if not.
*/
bool open_quotes(char* line_ptr);

/*
This method checks a there is a colon (:) without a label in the line.
It checks if the first character in the line is a colon.
Args:
:param line_ptr: the line to check.
:Return: True if the line is valid else false
*/
bool validate_prefix(char* line_ptr);

/*
This method checks if the command has the proper number of arguments.
Args:
cmd - the command name.
args - the command arguments.
Return:
true if the given number of args is valid and false if not.
*/
bool check_number_of_args(char* line_ptr);

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

/*
This method checks if a specific word is a reserved word. It's used to check label names
and valiedate them, by the opcode. if there's an opcode for the label name then it means
that it's a reserved word.
Args:
word - the word to check.
Return:
True if the word is a keyword and false if not.
*/
bool is_reserved_word(char* word);

#endif
