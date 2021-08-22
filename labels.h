#ifndef LABELS_H
#define LABELS_H

#include <stdbool.h>
#include <string.h>

#define LABEL_CHAR ':'

/*
 * Represent a Label
 *
 * Attributes:
 * label - Name of the label
 * value - Address of the label
 * is_code - Flag, false if the label point on data
 * is_entry - Flag, true if the label is an entry
 * is_external - Flag, true if the label is external
 */
typedef struct Label{
	char label[80];
	int value;
	unsigned int is_code: 1;
	unsigned int is_entry: 1;
	unsigned int is_external: 1;
} Label;

/*
 * Represent a Labels Table that map the labels
 * Based on the linked list pattern
 * Each node contain a label and a pointer to the next node
 *
 * Attributes:
 * label - Pointer to the label
 * next - Pointer to the next node
 */
typedef struct LabelsTable{
	Label *label;
	struct LabelsTable *next;
} LabelsTable;

/*
 * Check if a line contain a label
 *
 * Args:
 * line_ptr - Line to parse
 *
 * Return:
 * True if a string contains a label
 */
bool contain_label(char *line_ptr);

/*
 * Parse the label in a line
 *
 * Args:
 * line - The line to parse
 *
 * Return:
 * The label in a line
 */
char *get_label(char *line);

/*
 * Parse the label needed by an .entry line
 *
 * Args:
 * line_ptr - Line to parse
 *
 * Return:
 * The label wanted by the entry
 */
char *get_entry_label(char *line_ptr);

/*
 * Skip the label in a string
 *
 * Args:
 * line - Line to parse
 *
 * Return:
 * The line from after the label
 */
char *trim_label(char *line);

/*
 * Add a label to the labels table
 *
 * Args:
 * tbl_ptr - Pointer to the table that maps the labels
 * label - Label to add
 */
void add_label_to_table(LabelsTable *tbl_ptr, Label *label);

/*
 * Retrieve a label by its name
 *
 * Args:
 * tbl_ptr - Pointer to the table that maps the labels
 * name - Name of the label to find
 *
 * Return:
 * The label with the given name or null if it doesn't exist
 */
Label *get_label_by_name(LabelsTable *tbl_ptr, char *name);

/*
 * Retrieve the address of a label (given its name)
 *
 * Args:
 * tbl_ptr - Pointer to the table that maps the labels
 * name - Name of the label to find
 * frame_no -
 */
int get_label_addr(LabelsTable *tbl_ptr, char *name);

/*
 * Mark a label as an entry (given its name)
 *
 * Args:
 * tbl_ptr - Table mapping the labels
 * name - Name of the label to mark as entry
 */
void mark_label_as_entry(LabelsTable *tbl_ptr, char *name);

/*
 * Create a label and add it to the table
 *
 * Args:
 * tbl_ptr - Pointer to the table mapping the labels
 * addr - Address of the label
 * name - Name of the label
 * is_code - See is_code flag in Label
 * is_entry - See is_entry flag in Label
 * is_external - See is_external flag in Label
 */
Label *create_label(LabelsTable *tbl_ptr, int addr, char *name, int is_code, int is_entry, int is_external);

/*
 * Add a data instruction to a labels table
 *
 * Args:
 * tbl_ptr - Table that map the labels
 * addr - Address of the data instruction (DC)
 * label_name - Name of the label of the line
 */
void label_data_instruction(LabelsTable *tbl_ptr, int addr, char *label_name);

/*
 * Add a code instruction to a labels table
 *
 * Args:
 * tbl_ptr - Table that map the labels
 * addr - Address of the data instruction (IC)
 * label_name - Name of the label of the line
 */
void label_code_instruction(LabelsTable *tbl_ptr, int addr, char *label_name);

/*
 * Add an external variable to a labels table
 *
 * Args:
 * tbl_ptr - Table that map the labels
 * name - Name of the variable
 */
void add_external_variable(LabelsTable *tbl_ptr, char *label_name);

/*
 * Add an offset to every data label
 *
 * Args:
 * tbl_ptr - Table that map the labels
 * offset - offset to add to every data label
 */
void add_data_offset(LabelsTable *tbl_ptr, int offset);

#endif
