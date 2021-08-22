/*
 * First pass of the assembling, map every label to its corresponding address
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "errors.h"
#include "instructions.h"
#include "utils.h"
#include "globals.h"
#include "labels.h"
#include "second_pass.h"

/*
 * Boolean flags
 * has_label - True if the line contain a label
 * is_data_instruction - True if the line is a data instruction
 */
typedef struct Flags{
    unsigned int has_label : 1;
    unsigned int is_data_instruction : 1;
} Flags;


void first_pass(char *fname){
    Flags flags;

	FILE *fp; /* File pointer */
	char *line_ptr; /* Line holder buffer */
	size_t line_len; /* Max Length of a line in a file */
	size_t read_cnt; /* Number of character retrieved on a line */

    int ic, dc; /* Instruction counter, Data counter */
    char *label, *var_name; /* Store temporary strings */

	LabelsTable *labels_table; /* Holds the list of labels */

	/* Init variables */
    ic = 100; /* IC always start from 100 */
    dc = 0;
    line_ptr = (char *) calloc(LINE_MAX_SIZE, sizeof(char));
	line_len = LINE_MAX_SIZE;

	labels_table = (LabelsTable *) calloc(1, sizeof(LabelsTable));

	/* Open file */
	fp = fopen(fname, "r");

	/* Check if the file is valid */
	if (!fp){
		printf("Bad file: %s\n", fname);
        raise_error(NULL);
	}

	/* Loop - Read lines and parse them */
	while ((read_cnt = get_line_wout_spaces(&line_ptr, &line_len, fp)) != -1) {

        /* Reinitialize flags */
        flags.has_label = flags.is_data_instruction = 0;

        /* Clean the string */
        line_ptr = clean_str(line_ptr);

        /* Ignore every irrelevant line (comments/empty/etc..) */
        if (!relevant_line(line_ptr))
            continue;

        /* If the line is labelled, mark it and parse the label, then skip it */
        if (contain_label(line_ptr)){
            flags.has_label = 1; /* Mark the presence of the label */
            label = get_label(line_ptr); /* Parse the label */
            line_ptr = trim_label(line_ptr); /* Skip the label */
        }

        /*
         * Handle data instruction commands:
         * Calculate how many cells this data instruction will need
         * Label it if we need to
         */
        if (is_data_instruction(line_ptr)){
            flags.is_data_instruction = 1; /* Mark the line as data instruction */

            /* If the line contains a label, add it to the labels table */
            if (flags.has_label){
                label_data_instruction(labels_table, dc, label);
            }

            /* Update Data Counter */
            dc += get_required_cells(line_ptr);

            /* Go to next line */
            continue;
        }

        /* If it's a .entry instruction, ignore it (We will take care of it in the 2nd pass) */
        if (is_entry_instruction(line_ptr))
            continue;

        /* Handle external instruction */
        if (is_external_instruction(line_ptr)){

            /* Extract the variable name from the line */
            var_name = parse_external_var_name(line_ptr);

            /* Add this instruction as an external label */
            add_external_variable(labels_table, var_name);
            continue;
        }

        /* If we get here, it's an code instruction */
        if (flags.has_label) /* If there is a label, add it */
            label_code_instruction(labels_table, ic, label);

        ic += 4;
    }

    /*
     * Because we want to put every data definition at the end
     * of the binary output file, add IC to every labelled data
     */
    add_data_offset(labels_table, ic);

	/* Close the file */
	fclose(fp);

    /* Start second pass */
    second_pass(fname, labels_table, ic-100, dc);
}
