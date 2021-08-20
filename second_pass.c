#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "encoder.h"
#include "second_pass.h"
#include "instructions.h"
#include "labels.h"
#include "utils.h"
#include "globals.h"

void second_pass(char *fname, LabelsTable *labels_table_ptr, char *of, int dc_offset){
	FILE *fp;
    char *line_ptr;

    size_t line_len;
    ssize_t read_cnt;

    char *label;

    char *tmp_file; /* Temporary file to store data lines */

    int ic, dc;

    BITMAP_32 *bitmap;

    int processed_data; /* Counter to how many data cells were processed, data is dumped at 4 */
    BITMAP_32 *data_bitmap; /* Hold the next data bitmap to be dumped */

    /* Init variables */
    ic = 100;
    dc = 100 + dc_offset;
	line_len = LINE_MAX_SIZE;
    line_ptr = (char *) calloc(line_len, sizeof(char));

	/* Check if the file is valid */
	fp = fopen(fname, "r");
	if (!fp){
		printf("Bad file: %s\n", fname);
        return;
    }

	while ((read_cnt = getline(&line_ptr, &line_len, fp)) != -1) {
        /* Clean the line (remove unwanted characters) */
        line_ptr = clean_str(line_ptr);

        /* Ignore every irrelevant line (comments/empty...) */
        if (!relevant_line(line_ptr))
            continue;

        /* If it's an entry instruction - mark the symbol as entry */
        if (is_entry_instruction(line_ptr)){
            label = get_entry_label(line_ptr);
            mark_label_as_entry(labels_table_ptr, label);
            continue;
        }

        /* If the line is a labelled line, skip the label */
        if (contain_label(line_ptr)){
            /* Skip the label itself */
            while (*line_ptr++ != ':'){}
            /* Skip the whitespaces after the label */
            while ( isspace(*line_ptr) )
                line_ptr++;
        }

        /* If it's a data instruction, add the data to the memory */
        if (is_instruction(line_ptr)){
            tmp_dump_data_instruction(line_ptr, dc, data_bitmap); /* TODO - Big one */
            dc += get_required_cells(line_ptr);
            continue;
        }

        /* === If we got here, then it's a code instruction === */

        /* Encode the line to binary */
        bitmap = encode_instruction_line(line_ptr, labels_table_ptr, ic);

        ic += 4;

        /* Add the bitmap to the file */
        dump_bitmap(bitmap, of, ic);
    }

    /* Merge the temporary file to the output file */
    merge_tmp_data_file();

	fclose(fp);
}
