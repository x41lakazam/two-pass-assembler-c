#ifndef LABELS_H
#define LABELS_H

#include <stdbool.h>
#include <string.h>

#define LABEL_CHAR ":"


typedef struct LabelsTable{

} LabelsTable;

/*
 * Return the label in a line
 */
char *get_label(char *line);

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

#endif
