#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include "labels.h"

/*
 * Second pass of the assembling, encode every line to the object (.ob) file.
 * Every code instruction (normal command) line is directly dumped into the object file in the right format.
 * Every .entry instruction is directly dumped into an entries file (.ent)
 * Every use of an external label is dumped into a temporary externals file (.ext) that is renamed after
 * all the lines are parsed.
 * Data instruction are first encoded to a temporary file as raw bits. After reading the whole input file,
 * this temporary file will be parsed and converted to the right format in order to merge it to the object
 * file.
 *
 */
void second_pass(char *fname, LabelsTable *labels_table_ptr, int ic_size, int dc_size);
#endif
