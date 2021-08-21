#ifndef ENCODER_H
#define ENCODER_H
#include <stdio.h>
#include "labels.h"

#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) )

typedef int BITMAP_32[2];

/*
 * Instructions groups - R, I and J
 */
typedef enum {
	I,
	J,
	R
} InstructionsGroup;

/*
 * Translate a line instruction into bit maps
 */
BITMAP_32 *encode_instruction_line(char *line_ptr, LabelsTable *, int addr);

/*
 * Append 32 bits in file
 */
void dump_bitmap(BITMAP_32 *, char *, int, int);

void tmp_dump_data_instruction(char *line_ptr);

BITMAP_32 *build_R_instruction(int opcode, int rs, int rt, int rd, int funct_no);
BITMAP_32 *build_I_instruction(int opcode, int rs, int rt, int immed);
BITMAP_32 *build_J_instruction(int opcode, int is_reg, int addr);

/*
 * Return the difference between a label's address and the current frame index
 */
int get_label_addr_dist(char *lbl_name, LabelsTable *labels_tbl_ptr, int frame_addr);

InstructionsGroup get_instruction_group(char *cmd);

int get_opcode(char *cmd);

/*
 * Merge the temporary data file to the real .ob file
 */
void merge_tmp_data_file();

/*
 * Create all the temporary files needed for the encoding
 */
void create_tmp_files();

/*
 * Delete all the temporary files that were needed for the encoding
 */
void delete_tmp_files();

/*
 * Add an external label to the temporary external labels file
 */
void tmp_dump_external_label(char *lbl_name, LabelsTable *labels_table_ptr, int frame_no);

/*
 * Add an integer to the bitmap
 */
void add_obj_to_bitmap(int obj, int *start_ix, int size, BITMAP_32 *bitmap);

/*
 * Return the function id of an instruction (required by instructions of group R)
 * :param cmd_name: Name of the instruction
 */
int get_function_id(char *cmd_name);

/*
 * Print the content of a bitmap
 */
void print_bitmap_32(BITMAP_32 *bitmap);

void reverse_dump_bitmap(BITMAP_32 *bitmap, char *fname, int line_no, int bytes_to_dump);

void dump_entry_labels(LabelsTable *labels_tbl_ptr, char *of);

/*
 * Rename the temporary file containing the external labels into
 * a persistent one, with the right name.
 * :param external_of: Filename of the file containing the external labels
 */
void rename_externals_file(char *external_of);

/*
 * Clear every bit in a bitmap
 */
void reset_bitmap(BITMAP_32 *bitmap);
#endif
