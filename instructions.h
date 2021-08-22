#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdbool.h>

/*
 * Instructions groups - R, I and J
 */
typedef enum {
	I,
	J,
	R
} InstructionsGroup;

/*
 * Check if a line is a data instruction
 *
 * Args:
 * line_ptr - Line to parse
 *
 * Return:
 * True if a line is a data instruction line else false
 */
bool is_data_instruction(char *line_ptr);

/*
 * Return the name of the instruction in a line
 *
 * Args:
 * line_ptr - Line to parse
 *
 * Return:
 * Data instruction in the line
 */
char *get_instruction(char *line_ptr);

/*
 * Check if a line is an .entry instruction
 *
 * Args:
 * line_ptr - Line to parse
 *
 * Return:
 * True if a line is an entry instruction
 */
bool is_entry_instruction(char *line_ptr);

/*
 * Check if a line is a .external instruction
 *
 * Args:
 * line_ptr - Line to parse
 *
 * Return:
 * True if a line is an external instruction
 */
bool is_external_instruction(char *line_ptr);

/*
 * Parse the variable of an .external instruction
 *
 * Args:
 * line_ptr - Line to parse
 *
 * Return:
 * The name of the external variable
 */
char *parse_external_var_name(char *line_ptr);

/*
 * Calculate the number of bytes required by a line
 *
 * Args:
 * line_ptr - Line to parse
 *
 * Return:
 * The number of required memory cells by an instruction line
 * (each cell is 4 bytes)
 */
int get_required_cells(char *line_ptr);

/*
 * Check if a line is relevant
 *
 * Args:
 * line_ptr - The line to parse
 *
 * Return:
 * False if a line is irrelevant (empty or commented out) else True
 */
bool relevant_line(char *line_ptr);

/*
 * Parse the command name in an instruction line and put it in <buf>
 *
 * Args:
 * line_ptr - The instruction line string
 * buf - Buffer where the command name will be written
 */
void get_cmd_name(char *line_ptr, char *buf);

/*
 * Return the instruction group of a command
 *
 * Args:
 * cmd - The command name
 *
 * Return:
 * The instruction group associated with <cmd>
 */
InstructionsGroup get_instruction_group(char *cmd);

/*
 * Return the opcode associated with a command
 * Args:
 * cmd - The command
 *
 * Return:
 * The opcode of <cmd>
 */
int get_opcode(char *cmd);

/*
 * Return the function id of an instruction (required by instructions of group R)
 *
 * Args:
 * cmd_name - Name of the instruction (should be of group R)
 *
 * Return:
 * function id associated with <cmd_name>
 */
int get_function_id(char *cmd_name);

/*
 * Check if an instruction is a code instruction.
 * Actually check if a lineis not all the other types of lines
 */
bool is_code_instruction(char *line_ptr);

#endif
