/*
 * Instructions related functions
 */

#include <stdbool.h>
#include "utils.h"

char *instructions[] = {
    ".db",
    ".dh",
    ".dw",
    ".asciz"
};

int instructions_cnt = 4;

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
