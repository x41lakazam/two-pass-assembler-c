#include <stdio.h>
#include "encoder.h"
#include "labels.h"
#include "globals.h"

int R_cmds_len = 8;
char R_cmds[8][5] = {
	"add",
	"sub",
	"and",
	"or",
	"nor",
	"move",
	"mvhi",
	"mvlo"
};

int I_cmds_len = 15;
char I_cmds[15][5] = {
	"addi",
	"subi",
	"andi",
	"ori",
	"nori",
	"bne",
	"beq",
	"blt",
	"bgt",
	"lb",
	"sb",
	"lw",
	"sw",
	"lh",
	"sh"
};

int J_cmds_len = 4;
char J_cmds[4][5] = {
	"jmp",
	"la",
	"call",
	"stop"
};

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

void tmp_dump_data_instruction(char *line_ptr, int addr){
	/* TODO */
}

void merge_tmp_data_file(){
	/* TODO */
}

void get_cmd_name(char *line_ptr, char *buf){
    int i = 0;

    while (i < 4 && *line_ptr != ' ')
        *buf++ = *line_ptr++;
}

InstructionsGroup get_instruction_group(char *cmd_name){
    /* Assuming cmd_name is a valid command name */
    int i;

    /* Instruction is in R group ? */
    for (i=0; i<R_cmds_len; i++)
        if (STREQ(R_cmds[i], cmd_name)) return R;

    /* Instruction is in I group ? */
    for (i=0; i<I_cmds_len; i++)
        if (STREQ(I_cmds[i], cmd_name)) return I;

    /* Else instruction is in J group */
    return J;
}

int get_opcode(char *cmd_name){
	return 0; /* TODO */
}

int translate_label(char *lbl_name, LabelsTable *labels_tbl_ptr, int frame_addr){
    Label *lbl;
    int lbl_addr;

    /* Retrieve the right label */
    lbl = get_label_by_name(labels_tbl_ptr, lbl_name);

    /* Get its address */
    lbl_addr = lbl->value;

    /* Return the difference between the label's address and the frame's address */
    return lbl_addr - frame_addr;
}

BITMAP_32 *encode_instruction_line(char *line_ptr, LabelsTable *labels_table_ptr, int frame_no){
	BITMAP_32 *bitmap;
	InstructionsGroup instr_grp;
	char cmd_name[5];

	int opcode;
	int rs,rt,rd; /* Hold registers numbers */
	int immed, addr, funct_no; /* Integer buffers specific to each group*/
	int is_reg = 0; /* register flag for J group */
	char* token;
	char* params; /* the cmd line without the cmd itself */

	params = strchr(line_ptr, ' '); 
	if(params != NULL)
	{
		params = params + 1; 
	}

	get_cmd_name(line_ptr, cmd_name);
	opcode = get_opcode(cmd_name);
	instr_grp = get_instruction_group(cmd_name);

	switch (instr_grp) {
	/* TODO:
	 * In each case,
	 * Parse the right parameters (according to the group format) and call build_<>_instruction().
	 * If a parameter is a label, use:
	 * translate_label(<label_name>, labels_table_ptr, frame_no)
	 * else just send it as it is.
	 */
	case I:
		token = strtok(params, ","); 
		if (!(token + 1))
		{
			rs = 0; 
			break; 
		}
		rs = atoi(token) + 1; 

		token = strtok(NULL, ",");
		if (!(token))
		{
			immed = 0; 
			break; 
		}
		immed = atoi(token); 

		token = strtok(NULL, ",");
		if (!(token + 1))
		{
			rt = 0; 
			break; 
		}
		rt = atoi(token) + 1; 
	        bitmap = build_I_instruction(opcode, rs, rt, immed); 
	        break;
	case R:
		token = strtok(params, ","); 
		if (!(token + 1))
		{
			rs = 0; 
			break; 
		}
		rs = atoi(token) + 1; 

		token = strtok(NULL, ",");
		if (!(token + 1))
		{
			rt = 0; 
			break; 
		}
		rt = atoi(token) + 1; 

		token = strtok(NULL, ",");
		if (!token)
		{
			rd = 0;
			break; 
		}
		rd = atoi(token) + 1;
 
            	bitmap = build_R_instruction(opcode, rs, rt, rd, funct_no); 
            	break;
	case J:
		if (strchr(params, '$'))
		{
			is_reg = 1; 
			addr = atoi(params + 1);
		}
		else
		{
			addr = translate_label(params, labels_table_ptr, frame_no); 
		}
		bitmap = build_J_instruction(opcode, is_reg, addr); 
	    	break;
	}

	return bitmap;
}

void print_bitmap_32(BITMAP_32 *bitmap){
    int i;
    int bit;

    /* For each bit, if it's on print a 1, else print a 0 */
    for (i=1; i <= 31; i++)
        TestBit(*bitmap, i) ? printf("1") : printf("0");

    printf("\n");
}

void add_obj_to_bitmap(int obj, int *start_ix, int size, BITMAP_32 *bitmap){
    int bit_shift;

    /*
     * Do <size> times:
     * Compute the next bit ix, if the bit is turned on, set the corresponding
     * bit in the bitmap
     */
    for (bit_shift = size; bit_shift >= 0; bit_shift--){
        if ( ((obj >> bit_shift) & 1) == 1 ) /* Compute the bit at this index */
            SetBit(*bitmap, *start_ix); /* Set it in the bitmap if we need to */

        (*start_ix)++; /* Increment index */

    }
}

BITMAP_32 *build_R_instruction(int opcode, int rs, int rt, int rd, int funct_no) {
    BITMAP_32 *bitmap;
    int bit_ix;

    /* Start at bit 0 (left one)*/
    bit_ix = 0;

    /* Encode opcode in 6 bits */
    add_obj_to_bitmap(opcode, &bit_ix, 6, bitmap);

    /* Encode rs in 5 bits */
    add_obj_to_bitmap(rs, &bit_ix, 5, bitmap);

    /* Encode rt in 5 bits */
    add_obj_to_bitmap(rt, &bit_ix, 5, bitmap);

    /* Encode rd in 5 bits */
    add_obj_to_bitmap(rd, &bit_ix, 5, bitmap);

    /* Encode funct_no in 5 bits */
    add_obj_to_bitmap(funct_no, &bit_ix, 5, bitmap);

    /* Return the bitmap */
    return bitmap;

}

BITMAP_32 *build_I_instruction(int opcode, int rs, int rt, int immed) {
    BITMAP_32 *bitmap;
    int bit_ix;

    /* Start at bit 0 (left one)*/
    bit_ix = 0;

    /* Encode opcode in 6 bits */
    add_obj_to_bitmap(opcode, &bit_ix, 6, bitmap);

    /* Encode rs in 5 bits */
    add_obj_to_bitmap(rs, &bit_ix, 5, bitmap);

    /* Encode rt in 5 bits */
    add_obj_to_bitmap(rt, &bit_ix, 5, bitmap);

    /* Encode immed in 16 bits */
    add_obj_to_bitmap(immed, &bit_ix, 16, bitmap);

    return bitmap;
}

BITMAP_32 *build_J_instruction(int opcode, int is_reg, int addr) {
    BITMAP_32 *bitmap;
    int bit_ix;

    /* Start at bit 0 (left one)*/
    bit_ix = 0;

    /* Encode opcode in 6 bits */
    add_obj_to_bitmap(opcode, &bit_ix, 6, bitmap);

    /* Encode register flag in 1 bit */
    add_obj_to_bitmap(is_reg, &bit_ix, 1, bitmap);

    /* Encode address in 25 bits */
    add_obj_to_bitmap(addr, &bit_ix, 25, bitmap);

    return bitmap;
}
