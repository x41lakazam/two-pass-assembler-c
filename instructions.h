#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdbool.h>

/*
 * Return True if a line is an instruction
 */
bool is_instruction(char *line_ptr);

/*
 * Return the name of the instruction
 */
char *get_instruction(char *line_ptr);

/*
 * Return True if a line is an entry instruction
 */
bool is_entry_instruction(char *line_ptr);

/*
 * Return True if a line is an external instruction
 */
bool is_external_instruction(char *line_ptr);

/*
 * Return the name of the external variable
 */
char *parse_external_var_name(char *line_ptr);

/*
 * Return the number of required memory cells
 * by an instruction line
 */
int get_required_cells(char *line_ptr);

/*
 * Return False if a line is irrelevant
 * (empty or commented out)
 */
bool relevant_line(char *s);

#endif
