#include <stdio.h>
#include "encoder.h"

void dump_bitmap(BITMAP_32 *bitmap, char *fname, int line_no) {
	int i;
	unsigned int j;
    FILE *fp;

    fp = fopen(fname, "a");

	for (i = 0; i < 4; i++)
	{
		for (j = 1 << 8; j > 0; j = j / 2)
		{
			(*bitmap[i] & j) ? fprintf(fp, "1") : fprintf(fp, "0");
		}

    fclose(fp);
	}
}

BITMAP_32 *encode_instruction_line(char *line_ptr, LabelsTable *labels_table_ptr){

}


