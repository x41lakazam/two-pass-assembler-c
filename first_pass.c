#include <stdio.h>
#include <stdlib.h>

#define LINE_MAX_SIZE 81

void first_pass(char *fname, struct Node *external_labels_ptr){
	FILE *fp; /* File pointer */
	char *line_ptr; /* Line holder */
	size_t line_len; /* Max Length of a line in a file */
	ssize_t read_cnt; /* Number of character retrieved on a line */

	struct Node *labels_table_ptr;

	/* Init variables */
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
        printf("Retrieved line of length %zu:\n", read_cnt);
        printf(line_ptr);
    }


	/* Close the file */
	fclose(fp);
}
