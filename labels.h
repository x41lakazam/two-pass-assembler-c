#ifndef LABELS_H
#define LABELS_H

#include <stdbool.h>
#include <string.h>

#define LABEL_CHAR ':'

typedef struct Label{
	char label[80]; /* Name of the label */
	int value; /* Address of the label */
	unsigned int is_code: 1; /* If false --> label points on data */
	unsigned int is_entry: 1; /* True if the label is an entry */
	unsigned int is_external: 1; /* True if the label is external */
} Label;

typedef struct LabelsTable{
	Label *label;
	struct LabelsTable *next;
} LabelsTable;

/*
 * Return the label in a line
 */
char *get_label(char *line);

/*
 * Return the line from after the label
 */
char *trim_label(char *line);

/*
 * Add a data instruction to a labels table
 */
void label_data_instruction(LabelsTable *, int, char *);

/*
 * Add a code instruction to a labels table
 */
void label_code_instruction(LabelsTable *, int, char *);

/*
 * Return the label wanted by the entry (TODO - Maya)
 */
char *get_entry_label(char *line_ptr);

void mark_label_as_entry(LabelsTable *, char *);

/*
 * Add a label to the labels table
 */
void add_label_to_table(LabelsTable *tbl_ptr, Label *label);

/*
 * Return the label with the given name
 * or null if it doesn't exist
 */
Label *get_label_by_name(LabelsTable *tbl_ptr, char *name);

void add_external_variable(LabelsTable *tbl_ptr, char *name);

/*
 * Create a label and add it to the table
 */
Label *create_label(LabelsTable *tbl_ptr, int addr, char *name, int is_code, int is_entry, int is_external);

void add_external_variable(LabelsTable *tbl_ptr, char *label_name);

void add_data_offset(LabelsTable *tbl_ptr, int offset);

int get_label_addr(LabelsTable *tbl_ptr, char *name, int frame_no);

#endif
