#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdbool.h>

/*
 * Check if a line is a data instruction
 *
 * Args:
 * line_ptr - Line to parse
 *
 * Return:
 * True if a line is a data instruction line else false
 */
bool is_instruction(char *line_ptr);

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

#endif
