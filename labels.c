#include <stdlib.h>
#include <ctype.h>
#include "errors.h"
#include "globals.h"
#include "labels.h"
#include "utils.h"

/*
 * Return True if a string contains a label
 */
bool contain_label(char *s){
    while (*s){
        if (*s++ == ':')
            return true;
    }
    return false;
}

char *get_label(char *line) {
	char* label;

	/*strtok changes the string */
 	char* line_cpy = (char *) calloc(LINE_MAX_SIZE, sizeof(char));
	strcpy(line_cpy, line);

	label = strtok(line_cpy, ":"); /* parse the label */
	return label;
}

char *get_entry_label(char *line_ptr){
    char *ret;
	char *s = (char *) calloc(LINE_MAX_SIZE, sizeof(char)); /* Running ptr */

    ret = s;	/* ret hold the beginning address of s */

	/* Jump to the next word */
	while ( !isspace(*line_ptr) ) /* Move to next whitespace */
        line_ptr++;
	while ( isspace(*line_ptr) ) /* Move to next char */
        line_ptr++;

	/* Copy the next word */
	while ( *line_ptr && !isspace(*line_ptr) )
		*s++ = *line_ptr++;

	return ret;
}

char *trim_label(char *line){
	while (*line++ != LABEL_CHAR) {}
	line = trim_whitespaces(line);
	return line;
}

void add_label_to_table(LabelsTable *tbl_ptr, Label *label){
	LabelsTable *last_node;
	LabelsTable *new_node;

	if (tbl_ptr == NULL || label == NULL)
		return;


    /* Initialize last_node on the beginning of the table */
	last_node = tbl_ptr;

    /* If the table is empty, add the new node as the first node */
    if (last_node->label == NULL){
        last_node->label = label;
        return;
    }

    /* Else if the table is not empty, go through it until the last node */
	while (last_node->next != NULL)
		last_node = last_node->next;

    /* Create a new node containing the label */
	new_node = (LabelsTable *) calloc(1, sizeof(LabelsTable));
	new_node->label = label;
	new_node->next = NULL;

    /* Add the new node as the next node of the last node */
	last_node->next = new_node;
    return;
}

Label *get_label_by_name(LabelsTable *tbl_ptr, char *name){
    LabelsTable *curr_node;
    if (tbl_ptr == NULL || name == NULL)
        return NULL;

    curr_node = tbl_ptr;
    while (curr_node){

        /* If the node's label's name match, return it */
        if (STREQ(curr_node->label->label, name))
            return curr_node->label;

        /* Else move on to the next node */
        curr_node = curr_node->next;
    }

    /* No matching node has been found */
    return NULL;
}

int get_label_addr(LabelsTable *tbl_ptr, char *name, int frame_no){
    Label *lbl;
    lbl = get_label_by_name(tbl_ptr, name);
    if (lbl == NULL)
        raise_error("Label doesn't exist");

	/* If the label is external, return 0 */
	if (lbl->is_external){
		return 0;
	}

    return lbl->value;
}

void mark_label_as_entry(LabelsTable *tbl, char *name){
    Label *label;

    /* Get the right label */
    label = get_label_by_name(tbl, name);

    /* If label is none, then no label match this name */
    if (!label)
        return;

    label->is_entry = 1;
}

Label *create_label(LabelsTable *tbl_ptr, int addr, char *name, int is_code, int is_entry, int is_external){
    Label *label;

    /* Create a label */
	label = (Label *) calloc(1, sizeof(Label));

    /* Set members values */
	label->value = addr;
	label->is_code = is_code;
	label->is_entry = is_entry;
	label->is_external = is_external;

    if (name)
	    strcpy(label->label, name);

    /* Add this label to the labels table */
    add_label_to_table(tbl_ptr, label);

    return label;
}

void label_data_instruction(LabelsTable *tbl_ptr, int addr, char *label_name){
	create_label(tbl_ptr, addr, label_name, 0, 0, 0);
}

void label_code_instruction(LabelsTable *tbl_ptr, int addr, char *label_name){
	create_label(tbl_ptr, addr, label_name, 1, 0, 0);
}

void add_external_variable(LabelsTable *tbl_ptr, char *label_name){
	create_label(tbl_ptr, 0, label_name, 0, 0, 1);
}

void add_data_offset(LabelsTable *tbl_ptr, int offset){
    /* Iterate over all the nodes and add an offset to the data ones */
    while (tbl_ptr){

        /* If the node is a data one, add <offset> to its value */
        if (tbl_ptr->label->is_code == 0)
            tbl_ptr->label->value += offset;

        /* Move to the next node */
        tbl_ptr = tbl_ptr->next;
    }
}
