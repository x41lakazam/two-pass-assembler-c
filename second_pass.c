#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "encoder.h"
#include "second_pass.h"
#include "instructions.h"
#include "labels.h"
#include "utils.h"
#include "globals.h"

void second_pass(char *fname, LabelsTable *labels_table_ptr, int dc_offset){
	FILE *fp;
    char *line_ptr;

    size_t line_len;
    ssize_t read_cnt;

    char *label;

    char *tmp_file; /* Temporary file to store data lines */

    int ic, dc;

	char *file_basename; /* Base name of the processed file */
    char *main_of; /* main output file */
    char *entries_of; /* entries output file */
    char *external_of; /* externals output file */

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

    /* Create output files */
	file_basename = get_basename(fname);

    /* Main output file is file basename with .ob at the end */
    main_of = (char *) calloc(strlen(file_basename)+4, sizeof(char));
    strcpy(main_of, file_basename);
    strcat(main_of, ".ob");

    /* Entries output file is file basename with .ent at the end */
    entries_of = (char *) calloc(strlen(file_basename)+5, sizeof(char));
    strcpy(entries_of, file_basename);
    strcat(entries_of, ".ent");

    /* External output file is file basename with .ext at the end */
    external_of = (char *) calloc(strlen(file_basename)+5, sizeof(char));
    strcpy(external_of, file_basename);
    strcat(external_of, ".ext");

    /* Create the files */
    fopen(main_of, "w");
    fopen(entries_of, "w");
    fopen(external_of, "w");
    create_tmp_files();

	while ((read_cnt = getline(&line_ptr, &line_len, fp)) != -1) {
        /* Clean the line (remove unwanted characters) */
        line_ptr = clean_str(line_ptr);

        /* Ignore every irrelevant line (comments/empty...) */
        if (!relevant_line(line_ptr))
            continue;

		/* If it's an external instruction, ignore it */
		if (is_external_instruction(line_ptr))
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
            tmp_dump_data_instruction(line_ptr);
            continue;
        }

        /* === If we got here, then it's a code instruction === */

        /* Encode the line to binary */
        bitmap = encode_instruction_line(line_ptr, labels_table_ptr, ic);

        /* Add the bitmap to the file */
        dump_bitmap(bitmap, main_of, ic, 4);

        /* Increment instruction counter */
        ic += 4;
    }

    /* Merge the temporary data file to the output file */
    merge_tmp_data_file(main_of, dc_offset);

    /* Create entries file */
    dump_entry_labels(labels_table_ptr, entries_of);

    /* Create external file */
    rename_externals_file(external_of);

	fclose(fp);
    /* delete_tmp_files(); */
}
