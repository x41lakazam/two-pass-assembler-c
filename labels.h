#ifndef LABELS_H
#define LABELS_H

#include <stdbool.h>

typedef struct LabelsTable{

} LabelsTable;

/*
 * Return the label in a line
 */
char *get_label(char *);

/*
 * Add a data instruction to a labels table
 */
void label_data_instruction(LabelsTable *, int, char *);

/*
 * Add a code instruction to a labels table
 */
void label_code_instruction(LabelsTable *, int, char *);

#endif
