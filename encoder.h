#ifndef ENCODER_H
#define ENCODER_H
#include <stdio.h>
#include "labels.h"

#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) )

typedef int BITMAP_32[2];

/*
 * Append 4 bytes in file
 * The last byte in the bitmap is the first to be printed
 * Args:
 * bitmap - The bytes to dump
 * fname - Output filename
 * line_no - Number of the line that is dumped
 * bytes_to_dump - Number of bytes to read from the bitmap
 */
void dump_bitmap(BITMAP_32 *bitmap, char *fname, int line_no, int bytes_to_dump);

/*
 * Append 4 bytes in file
 * The first byte in the bitmap is the first to be printed
 * Args:
 * bitmap - The bytes to dump
 * fname - Output filename
 * line_no - Number of the line that is dumped
 * bytes_to_dump - Number of bytes to read from the bitmap
 */
void reverse_dump_bitmap(BITMAP_32 *bitmap, char *fname, int line_no, int bytes_to_dump);

/*
 * Dump a data instruction (.db, .dh...) to a temporary file, as raw bits
 * Args:
 * line_ptr - instruction to dump
 */
void tmp_dump_data_instruction(char *line_ptr);

/*
 * Add an external label to the temporary external labels file.
 * Args:
 * lbl_name - Name of the label
 * labels_table_ptr - Labels table that map this label
 * frame_no - Number of the line where this external label is needed
 */
void tmp_dump_external_label(char *lbl_name, LabelsTable *labels_table_ptr, int frame_no);

/*
 * Dump every entry labels to the entries output file (.ent)
 * The file is created from scratch
 *
 * Args:
 * labels_tbl_ptr - The table mapping all the labels
 * of - Name of the output file containing the entries
 */
void dump_entry_labels(LabelsTable *labels_tbl_ptr, char *of);

/*
 * Build an instruction of the R group in this format:
 * <opcode (6) | rs (5) | rt (5) | rd (5) | funct_no (5) | Empty (5)>
 *
 * Args:
 * opcode - The command opcode
 * rs - register S
 * rt - register T
 * rd - register D
 * funct_no - The function id of the command
 *
 * Return:
 * A bitmap encoding the instruction
 */
BITMAP_32 *build_R_instruction(int opcode, int rs, int rt, int rd, int funct_no);

/*
 * Rename the temporary file containing the external labels into
 * a persistent one, with the right name.
 * :param external_of: Filename of the file containing the external labels
 */
void rename_externals_file(char *external_of);

/*
 * Clear every bit in a bitmap
 *
 * Args:
 * bitmap - the bitmap to clear
 */
void reset_bitmap(BITMAP_32 *bitmap);

/*
 * Merge the temporary data file to the real .ob file
 * Also convert the raw bits written in the temporary data file
 * to the hexadecimal format of the object file
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
 * Return the difference between a label's address and the current frame index
 * Args:
 * lbl_name - Name of the label
 * labels_tbl_ptr - Table mapping this label
 * frame_addr - Address on which the distance should be calculated
 *
 * Return:
 * The distance between the label's address and the frame address (in bytes)
 */
int get_label_addr_dist(char *lbl_name, LabelsTable *labels_tbl_ptr, int frame_addr);

/*
 * Translate a line instruction into bit maps, following the format needed by each instruction
 *
 * Args:
 * line_ptr - Line to parse
 * labels_tbl_ptr - Table that map labels
 * addr - address of the instruction line (IC)
 *
 * Return:
 * A bitmap representing the encoded line
 */
BITMAP_32 *encode_instruction_line(char *line_ptr, LabelsTable *labels_tbl_ptr, int addr);

/*
 * Print the content of a bitmap
 *
 * Args:
 * bitmap - The bitmap to print
 */
void print_bitmap_32(BITMAP_32 *bitmap);

/*
 * Add an integer to a bitmap at a given index, the size of the obj can vary (up to 32 bits)
 *
 * Args:
 * obj - The obj to add
 * start_ix - Pointer to the start index where the obj should be added (from 0 to 31)
 * size - Size in bits of the added object (from 1 to 32)
 * bitmap - The bitmap where the object should be added
 */
void add_obj_to_bitmap(int obj, int *start_ix, int size, BITMAP_32 *bitmap);

/*
 * Build an instruction of the I group in this format:
 * <opcode (6) | rs (5) | rt (5) | immed (16)>
 *
 * Args:
 * opcode - The command opcode
 * rs - register S
 * rt - register T
 * immed - Immed field
 *
 * Return:
 * A bitmap encoding the instruction
 */
BITMAP_32 *build_I_instruction(int opcode, int rs, int rt, int immed);

/*
 * Build an instruction of the R group in this format:
 * <opcode (6) | is_reg (1) | addr (25)>
 *
 * Args:
 * opcode - The command opcode
 * is_reg - True if the address is a register number
 * addr - Address to use
 *
 * Return:
 * A bitmap encoding the instruction
 */
BITMAP_32 *build_J_instruction(int opcode, int is_reg, int addr);















#endif
