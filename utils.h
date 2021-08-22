/*
 * General utilitaries functions
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Reads an entire line from a stream, but ignore extra whitespaces
 * If the length of the line is bigger than <size>, the buffer will be reallocated
 * (if possible, otherwise an error will be printed)
 *
 * Based on getline()
 *
 * Args:
 * buffer - Buffer to store the line in
 * size - Number of chars to read
 * file - Stream to read from
 */
int get_line_wout_spaces(char **buffer, size_t *size, FILE *file);

/*
 * Return the basename of <filename>, meaning the name
 * of the file without extension
 *
 * Args:
 * filename - Name of the file
 *
 * Return:
 * Corresponding basename
 */
char *get_basename(char *filename);

/*
 * Remove every whitespace at the beggining of a string
 *
 * Args:
 * s - string to trim
 *
 * Return:
 * <s> without leading whitespaces
 */
char *trim_whitespaces(char *s);

/*
 * Check if a string starts with another
 *
 * Args:
 * s - The string on which to check if the prefix exist
 * t - the prefix
 *
 * Return:
 * true if <s> starts with <t>
 */
bool starts_with(char *s, char *t);

/*
 * Clean a string, remove every unwanted character.
 * Remove trailing whitespaces
 * Remove unwanted characters
 * Remove consecutive whitespaces
 *
 * After the clean there will be:
 * - no whitespace in the label section and after the label colon
 * - one space between the command and the arguments
 * - no whitespace between the arguments
 *
 * Args:
 * s - the string to clean
 *
 * Return:
 * The clean string
 */
char *clean_str(char *s);

/*
 * Build the name of the entries file (.ent)
 *
 * Args:
 * filename - Name of the base file (.as)
 *
 * Return
 * Name of the entries file
 */
char *get_entries_outfile(char *filename);

/*
 * Build the name of the externals file (.ext)
 *
 * Args:
 * filename - Name of the base file (.as)
 *
 * Return
 * Name of the externals file
 */
char *get_externals_outfile(char *filename);

#endif
