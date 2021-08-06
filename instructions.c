/*
 * Instructions related functions
 */

#include <stdbool.h>
#include "utils.h"


/*
 * Instructions table
 * Store every possible instruction
 */
char *instructions[] = {
    ".db",
    ".dh",
    ".dw",
    ".asciz"
};

/*
 * Store the number of available instructions
 */
int instructions_cnt = 4;

/*
 * Return True if a string is a valid instruction
 */
bool is_instruction(char *s){
    int i;
    /*
     * Not an instruction if it doesn't start
     * with a point
     */
    if (*s != '.')
        return false;

    for (i=0; i < instructions_cnt; i++){
        if (starts_with(s, instructions[i]))
            return true;
    }
    return false;
}
