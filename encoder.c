#include <stdio.h>
#include "encoder.h"

void dump_bitmap(BITMAP_32 *bitmap, FILE *f) {
/* assuming that f is already open */
	int i;
	unsigned int j;

	for (i = 0; i < 4; i++)
	{
		for (j = 1 << 8; j > 0; j = j / 2)
		{
			(*bitmap[i] & j) ? fprintf(f, "1") : fprintf(f, "0");
		}
		fprintf(f, "\n");
	}
}


