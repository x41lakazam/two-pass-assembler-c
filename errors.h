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
line_ptr - Line to parse
line_no - Number of the parsed line

Return:
true if the given number of args is valid and false if not.
*/
bool check_number_of_args(char* line_ptr, int line_no);

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

/*
 * Check that a command exists
 *
 * Args:
 * cmd_name - Command to check
 *
 * Return:
 * True if the command exists else false
 */
bool command_exists(char *cmd_name);

/*
 * Make validations on a label and print errors if there are any
 *
 * Args:
 * lbl_name - Name of the label
 *
 * Return:
 * True if the label is valid else false
 */
bool validate_label(char *lbl_name, int line_no);

/*
 * Check the registers on a code instruction line
 * Check that they are numbers and in the right range
 *
 * Args:
 * line_ptr - The line to parse
 * line_no - Number of the parsed line
 *
 * Return
 * True if everything went right else false
 */
bool check_registers(char *line_ptr, int line_no);

#endif
