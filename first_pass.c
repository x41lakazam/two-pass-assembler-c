#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "instructions.h"
#include "utils.h"
#include "globals.h"
#include "linked_list.h"


/*
 * Boolean flags
 */
typedef struct Flags{
    unsigned int has_label : 1;
    unsigned int is_instruction : 1;
} Flags;


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

/*
 * Return True if a line is relevant, meaning it's not a comment or empty
 */
bool relevant_line(char *s){

    s = trim_whitespaces(s);

    if (*s == '\n')
        return false;

    if (*s == COMMENT_CHAR)
        return false;

    return true;
}

/*
 * Perform first pass on a file
 */
void first_pass(char *fname, struct Node *external_labels_ptr){
    Flags flags; /*  */
    int ic, dc; /* Instruction counter, Data counter */
	FILE *fp; /* File pointer */
	char *line_ptr; /* Line holder */
	size_t line_len; /* Max Length of a line in a file */
	ssize_t read_cnt; /* Number of character retrieved on a line */

	struct Node *labels_table_ptr;

	/* Init variables */
    ic = 100;
    dc = 0;
    line_ptr = (char *) calloc(LINE_MAX_SIZE, sizeof(char));
	line_len = LINE_MAX_SIZE;

	/* Open file */
	fp = fopen(fname, "r");

	/* Check if the file is valid */
	if (!fp){
		printf("Bad file: %s\n", fname);
		return;
	}

	/* Loop - Read lines and process them */
	while ((read_cnt = getline(&line_ptr, &line_len, fp)) != -1) {
        printf(line_ptr);

        /* Reinitialize flags */
        flags.has_label = flags.is_instruction = 0;

        /* Remove every leading whitespaces */
        line_ptr = trim_whitespaces(line_ptr);

        /* Ignore every irrelevant line (comments/empty...) */
        if (!relevant_line(line_ptr))
            continue;

        /* Turn on the right flags */
        if (contain_label(line_ptr))
            flags.has_label = 1;

        if (is_instruction(line_ptr))
            flags.is_instruction = 1;

    }

	/* Close the file */
	fclose(fp);
}
