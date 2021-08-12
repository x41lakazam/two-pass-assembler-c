#include "globals.h"
#include "labels.h"
#include <stdlib.h>

char *get_label(char *line) {
	char* label;
 	char* line_cpy = (char *) calloc(LINE_MAX_SIZE, sizeof(char));
	strcpy(line_cpy, line);
	label = strtok(line_cpy, LABEL_CHAR);
	return label;
}
