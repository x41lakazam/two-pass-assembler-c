#include <stdio.h>
#include <stdlib.h>

#include "encoder.h"
#include "second_pass.h"
#include "instructions.h"
#include "labels.h"
#include "utils.h"

void second_pass(char *fname, LabelsTable *labels_table_ptr, char *of, int dc_offset){
	FILE *fp;
    char *line_ptr;

    size_t line_len;
    ssize_t read_cnt;

    char *label;

    char *tmp_file; /* Temporary file to store data lines */

    int ic, dc;

    ic = 100;
    dc = 100 + dc_offset;

    BITMAP_32 *bitmap;

	fp = fopen(fname, "r");
	/* Check if the file is valid */
	if (!fp){
		printf("Bad file: %s\n", fname);
        return;
    }

	while ((read_cnt = getline(&line_ptr, &line_len, fp)) != -1) {
        /* remove every leading whitespaces */
        line_ptr = trim_whitespaces(line_ptr);

        /* Ignore every irrelevant line (comments/empty...) */
        if (!relevant_line(line_ptr))
            continue;

        /* If it's an entry instruction - mark the symbol as entry */
        if (is_entry_instruction(line_ptr)){
            label = get_entry_label(line_ptr); /* TODO - Maya */
            mark_label_as_entry(labels_table_ptr, label);
            continue;
        }

        /* If it's a data instruction, add the data to the memory */
        if (is_instruction(line_ptr)){
            tmp_dump_data_instruction(line_ptr, dc); /* TODO - Big one */
            dc += get_required_cells(line_ptr);
            continue;
        }

        /* === If we got here, then it's a code instruction === */

        /* Encode the line to binary */
        bitmap = encode_instruction_line(line_ptr, labels_table_ptr, ic);

        ic += 4;

        /* Add the bitmap to the file */
        dump_bitmap(bitmap, of, ic); /* TODO - Maya */
    }

    /* Merge the temporary file to the output file */
    merge_tmp_data_file();

	fclose(fp);
}
