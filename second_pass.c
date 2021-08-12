#include <stdio.h>
#include <stdlib.h>

#include "labels.h"
#include "encoder.h"

void add_data_to_map(char *line_ptr, MemoryMap *map_ptr){
    return;
}

void second_pass(char *fname, LabelsTable *labels_table_ptr, FILE *of){
	FILE *fp;
    char *line_ptr;

    size_t line_len;
    ssize_t read_cnt;

    char *label;

    BITMAP_32 *bitmap;

    MemoryMap *data_map_ptr; /* TODO - Eyal */

	fp = fopen(fname, "r");
	/* Check if the file is valid */
	if (!fp){
		printf("Bad file: %s\n", fname);

	while ((read_cnt = getline(&line_ptr, &line_len, fp)) != -1) {
        /* remove every leading whitespaces */
        line_ptr = trim_whitespaces(line_ptr);

        /* Ignore every irrelevant line (comments/empty...) */
        if (!relevant_line(line_ptr))
            continue;

        /* If it's an entry instruction - mark the symbol as entry */
        if (is_entry_instruction(line_ptr)){
            label = get_entry_label(line_ptr); /* TODO - Maya */
            mark_label_as_entry(labels_table_ptr, label); /* TODO - Eyal */
            continue;
        }

        /* If it's a data instruction, add the data to the memory */
        if (is_instruction(line_ptr)){
            add_data_to_map(line_ptr, data_map_ptr); /* TODO - Big one */
            continue;
        }

        /* === If we got here, then it's a code instruction === */

        /* Encode the line to binary */
        bitmap = encode_instruction_line(line_ptr, labels_table_ptr, data_map_ptr); /* TODO - Bit one */

        /* Add the bitmap to the file */
        dump_bitmap(bitmap, of); /* TODO - Maya */
    }

	fclose(fp);
}
