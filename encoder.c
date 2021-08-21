#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "errors.h"
#include "utils.h"
#include "encoder.h"
#include "labels.h"
#include "globals.h"
#include "instructions.h"

#define TMP_DATA_MMAP_FILE "tmp_data_mmap.ob"
#define TMP_ENTRIES_MMAP_FILE "tmp_entries_mmap.ob"
#define TMP_EXTERNALS_MMAP_FILE "tmp_externals_mmap.ob"

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

void dump_bitmap(BITMAP_32 *bitmap, char *fname, int line_no, int bytes_to_dump) {
	int i, j;
    int bit, bytes_dumped;
    unsigned int as_int;
    FILE *fp;

    j = as_int = bytes_dumped = 0;
    fp = fopen(fname, "a");

    if (fp == NULL)
        raise_error("File doesn't exist");

    fprintf(fp, "%04d ", line_no);

    for (i = 31; i >= 0; i--){
        bit = TestBit(*bitmap, i);

        /* Add this bit to the integer */
        if (bit)
            as_int += (int) pow(2, j);

        j++;

        /* Every 8 bits: convert the decimal value to hexadecimal and print it in the file */
        if (j == 8){
            /* Dump the hex value */
            fprintf(fp, "%02X", as_int);

            /* Add a space if it's not the last byte */
            if (i>0)
                fprintf(fp, " ");

            /* Reset j and as_int */
            j = as_int = 0;

            if (bytes_dumped == bytes_to_dump)
                break;
        }
    }

    fprintf(fp, "\n");
    fclose(fp);
}

/*
 *
 * :param cell_count: (int) Number of cells filled by data instruction, everytime this number reach 4,
 * dump the line.
 */
void tmp_dump_data_instruction(char *line_ptr){
    char *token; /* Used for strtok */
    char *instruction_name; /* Name of the data instruction */
    char *params; /* Parameters of the lines */
    int size; /* Size of the encoded word in bits */
    int shift; /* Shift in the bits of the bitmap */
    int i, val; /* Temporary variables */
    FILE *fp;

	instruction_name = get_instruction(line_ptr);

	params = (char *) calloc(LINE_MAX_SIZE, sizeof(char));
	strcpy(params, line_ptr + strlen(instruction_name));
	params = trim_whitespaces(params);

    /* Skip the operation name */
    while ( !isspace(*line_ptr++) ) {}

    /* Skip every trailing whitespace (even if there shouldn't be) */
    while ( isspace(*line_ptr) )
        line_ptr++;

    fp = fopen(TMP_DATA_MMAP_FILE, "a");

    if (STREQ(instruction_name, ".asciz")){
        /* Encode every character between the quotes and dump them */

        /* Go to the quote */
        while (*line_ptr++ != '"') {}

        /* Parse until the closing quote */
        while (*line_ptr != '"'){
            for(i = 7; i >= 0; i-- )
                fprintf(fp, "%d", ( *line_ptr >> i ) & 1 ? 1 : 0 );

            line_ptr++;
        }
    }
    else{
        /* Encode every number  */
        if (STREQ(instruction_name, ".db"))
            size = 8;
        else if (STREQ(instruction_name, ".dh"))
            size = 16;
        else /* instruction is .dw*/
            size = 32;

		token = strtok(params, ",");
		while (token)
		{
            val = atoi(token);

            for(i = size-1; i >= 0; i-- )
                fprintf(fp, "%d", ( val >> i ) & 1 ? 1 : 0 );

			token = strtok(NULL, ",");
		}
    }

    fclose(fp);

}

void tmp_dump_external_label(char *lbl_name, LabelsTable *labels_table_ptr, int frame_no){
    FILE *fp;
    Label *lbl;

    lbl = get_label_by_name(labels_table_ptr, lbl_name);

    if (lbl == NULL)
        return; /* TODO raise error */

    fp = fopen(TMP_EXTERNALS_MMAP_FILE, "a");

    fprintf(fp, "%s %04d", lbl->label, frame_no);

    fclose(fp);
}

void tmp_dump_entry_labels(LabelsTable *labels_tbl_ptr){
    Label *lbl;
    FILE *fp;

    fp = fopen(TMP_ENTRIES_MMAP_FILE, "w");

    /* Iterate over each label and print the label in the file if it's an entry */
    while (labels_tbl_ptr != NULL){
        /* Retrieve label */
        lbl = labels_tbl_ptr->label;

        /* If the label is an entry, add it to the file in the right format */
        if (lbl->is_entry){
            fprintf(fp, "%s %04d\n", lbl->label, lbl->value);
        }
        /* Move on to the next node */
        labels_tbl_ptr = labels_tbl_ptr->next;
    }

    fclose(fp);
}

/*
 * Clear every bit in a bitmap
 */
void reset_bitmap(BITMAP_32 *bitmap);

void reset_bitmap(BITMAP_32 *bitmap){
    int i;
    /* Iterate over the 31 bits and clear each one of them */
    for (i=0; i <= 31; i++)
        ClearBit(*bitmap, i);
}

void merge_tmp_data_file(char *dst_fname, int dc_offset){
    BITMAP_32 *bitmap; /* Hold the bitmap to dump */
    int frame_no; /* Hold the current line number */
    char c; /* Hold   */
    int i;
    FILE *fp;

    bitmap = (BITMAP_32 *) calloc(1, sizeof(BITMAP_32));

    frame_no = dc_offset;

    fp = fopen(TMP_DATA_MMAP_FILE, "r");

    i=0;
    while ( (c = fgetc(fp)) != EOF){
        if (c == '1')
            SetBit(*bitmap, i);

        i++;

        /* Every 32 bits, dump the current bitmap and reset everything */
        if (i == 32){
            dump_bitmap(bitmap, dst_fname, frame_no, 4);
            frame_no += 4;
            reset_bitmap(bitmap);
            i=0;
        }
    }

    /* Dump the remaining bits */
    if (i != 0)
        dump_bitmap(bitmap, dst_fname, frame_no, 4);

}

void create_tmp_files(){
    fopen(TMP_DATA_MMAP_FILE, "w");
    fopen(TMP_ENTRIES_MMAP_FILE, "w");
    fopen(TMP_EXTERNALS_MMAP_FILE, "w");
}

void delete_tmp_files(){
    remove(TMP_DATA_MMAP_FILE);
    remove(TMP_ENTRIES_MMAP_FILE);
    remove(TMP_EXTERNALS_MMAP_FILE);
}

void get_cmd_name(char *line_ptr, char *buf){
    int i = 0;

    while (i < 4 && *line_ptr != ' '){
        *buf++ = *line_ptr++;
        i++;
    }
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
    if (STREQ(cmd_name, "add") ||
        STREQ(cmd_name, "sub") ||
        STREQ(cmd_name, "and") ||
        STREQ(cmd_name, "or")  ||
        STREQ(cmd_name, "nor"))
        return 0;
    else if (STREQ(cmd_name, "move") ||
             STREQ(cmd_name, "mvhi") ||
             STREQ(cmd_name, "mvlo"))
        return 1;
    else if (STREQ(cmd_name, "addi")) return 10;
    else if (STREQ(cmd_name, "subi")) return 11;
    else if (STREQ(cmd_name, "andi")) return 12;
    else if (STREQ(cmd_name, "ori")) return 13;
    else if (STREQ(cmd_name, "nori")) return 14;
    else if (STREQ(cmd_name, "bne")) return 15;
    else if (STREQ(cmd_name, "beq")) return 16;
    else if (STREQ(cmd_name, "blt")) return 17;
    else if (STREQ(cmd_name, "bgt")) return 18;
    else if (STREQ(cmd_name, "lb")) return 19;
    else if (STREQ(cmd_name, "sb")) return 20;
    else if (STREQ(cmd_name, "lw")) return 21;
    else if (STREQ(cmd_name, "sw")) return 22;
    else if (STREQ(cmd_name, "lh")) return 23;
    else if (STREQ(cmd_name, "sh")) return 24;
    else if (STREQ(cmd_name, "jmp")) return 30;
    else if (STREQ(cmd_name, "la")) return 31;
    else if (STREQ(cmd_name, "call")) return 32;
    else if (STREQ(cmd_name, "stop")) return 63;

    raise_error("Command doesn't exist");
    return -1;
}

int get_function_id(char *cmd_name){
    if (STREQ(cmd_name, "add")) return 1;
    else if (STREQ(cmd_name, "sub")) return 2;
    else if (STREQ(cmd_name, "and")) return 3;
    else if (STREQ(cmd_name, "or")) return 4;
    else if (STREQ(cmd_name, "nor")) return 5;
    else if (STREQ(cmd_name, "move")) return 1;
    else if (STREQ(cmd_name, "mvhi")) return 2;
    else if (STREQ(cmd_name, "mvlo")) return 3;

    raise_error("Command doesn't exist");
    return -1;
}

int get_label_addr_dist(char *lbl_name, LabelsTable *labels_tbl_ptr, int frame_addr){
    Label *lbl;
    int lbl_addr;

    /* Retrieve the right label */
    lbl = get_label_by_name(labels_tbl_ptr, lbl_name);

	/* If the label is external, return 0 */
	if (lbl->is_external == 1)
		return 0;

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

    bitmap = (BITMAP_32 *) calloc(1, sizeof(BITMAP_32));

	params = strchr(line_ptr, ' ');
	if(params != NULL)
	{
		params = params + 1;
	}

	get_cmd_name(line_ptr, cmd_name);
	opcode = get_opcode(cmd_name);
	instr_grp = get_instruction_group(cmd_name);

    /* Parse the line and encode it accordingly to the operation's group */
	switch (instr_grp) {
		/*
		 * I group:
		 * If the operation is one of bne, beq, blt or bgt,
		 * the format is "OP $rs, $rt, LABEL"
		 * else it will be in the format "OP $rs, Immed (const), $rt"
		 */
        case I:
            /* Parse first register */
            token = strtok(params, ",");
            rs = atoi(token+1);

            if (STREQ(cmd_name, "bne") ||
                STREQ(cmd_name, "beq") ||
                STREQ(cmd_name, "blt") ||
                STREQ(cmd_name, "bgt")
                ){

                /* Parse second register */
                token = strtok(NULL, ",");
                rt = atoi(token + 1);

                /* Parse the label */
                token = strtok(NULL, ",");
                immed = get_label_addr_dist(token, labels_table_ptr, frame_no);

				/* Check if label is external */
				if (immed == 0){
					/* TODO: ERROR - Label should not be external */
				}
            }
            else{
                /* Parse immed (constant) */
                token = strtok(NULL, ",");
                immed = atoi(token);

                /* Parse rt */
                token = strtok(NULL, ",");
                rt = atoi(token+1);
            }

            /* Build bitmap */
            bitmap = build_I_instruction(opcode, rs, rt, immed);
            break;

		/*
		 * R group:
		 * Every command with opcode 0 need 3 arguments, while commands
		 * with opcode 1 only need 2.
         * if (is_external_instruction(line_ptr))
         * continue;
         *
		 */
        case R:
            /* Get function number required by R group instructions */
            funct_no = get_function_id(cmd_name);

            /* Parse first register */
            token = strtok(params, ",");
            rs = atoi(token+1);

            /* Parse second register */
            token = strtok(NULL, ",");
            rt = atoi(token+1);

            /* Parse third register only if opcode is 0, else set it to 0 */
            if (opcode == 0){
                token = strtok(NULL, ",");
                rd = atoi(token+1);
            }
            else
                rd = 0;

            /* Build final bitmap */
            bitmap = build_R_instruction(opcode, rs, rt, rd, funct_no);
            break;

		/*
		 * J group:
		 * If the command is a JMP command, the parameter can either be
		 * a register or a label
		 * If the command is a LA or CALL command, then the parameter can
		 * only be a label
		 * If the command is a STOP command, then it's not followed by anything
		 * and the bitmap will be full of 0, except for the opcode
		 */
        case J:

			/* If - Command is stop */
			if (STREQ(cmd_name, "stop"))
				addr = 0;

			/* Else if - Command is jmp and the argument is a register */
			else if (*params == '$' && STREQ(cmd_name, "jmp")){
                is_reg = 1;					/* Turn the register flag on */
                addr = atoi(params + 1);    /* Parse the value of the register index */
            }

			/* Else - Command is not stop and the argument is a label */
			else {
                /* Set addr to be the address the label points on */
                addr = get_label_addr(labels_table_ptr, params, frame_no);
                if (addr == 0)
                    tmp_dump_external_label(params, labels_table_ptr, frame_no);

            }

            /* Build final bitmap */
            bitmap = build_J_instruction(opcode, is_reg, addr);
            break;
        }

	return bitmap;
}

void print_bitmap_32(BITMAP_32 *bitmap){
    int i;
    int bit;

    /* For each bit, if it's on print a 1, else print a 0 */
    for (i=0; i <= 31; i++)
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
    for (bit_shift = size-1; bit_shift >= 0; bit_shift--){
        if ( ((obj >> bit_shift) & 1) == 1 ) /* Compute the bit at this index */
            SetBit(*bitmap, *start_ix); /* Set it in the bitmap if we need to */

        (*start_ix)++; /* Increment index */
    }
    /* (*start_ix)--; */
}

BITMAP_32 *build_R_instruction(int opcode, int rs, int rt, int rd, int funct_no) {
    BITMAP_32 *bitmap;
    int bit_ix;

    bitmap = (BITMAP_32 *) calloc(1, sizeof(BITMAP_32));

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

    bitmap = (BITMAP_32 *) calloc(1, sizeof(BITMAP_32));
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

    bitmap = (BITMAP_32 *) calloc(1, sizeof(BITMAP_32));
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
