#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdbool.h>

/*
 * Perform first pass on a file:
 * - Saving each defined/used label into a labels table
 * - Calculate IC and DC counters in order to build a
 *   memory map and calculate the address of each label
 *
 * :param fname: Name of the file to parse
 */
void first_pass(char *fname);

#endif
