#ifndef ENCODER_H
#define ENCODER_H
#include <stdio.h>
#include "labels.h"


#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) )

typedef int BITMAP_32[4];
typedef struct MemoryMap{

} MemoryMap;

/*
 * Translate a line instruction into bit maps
 */
BITMAP_32 *encode_instruction_line(char *line_ptr, LabelsTable *);

/*
 * Add data entry to the memory map
 */
void add_data_to_map(char *, MemoryMap *);

/*
 * Append 32 bits in file
 */
void dump_bitmap(BITMAP_32 *, char *, int);

void tmp_dump_data_instruction(char *line_ptr, int dc);

BITMAP_32 *build_R_instruction(int opcode, int rs, int rt, int rd, int funct_no);
BITMAP_32 *build_I_instruction(int opcode, int rs, int rt, int immed);
BITMAP_32 *build_J_instruction(int opcode, int is_reg, int addr);

#endif
