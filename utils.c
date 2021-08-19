/*
 * Utilitaries functions
 */

#include <string.h>
#include <stdbool.h>

#include "globals.h"

/*
 * Remove every leading whitespace
 */
char *trim_whitespaces(char *s){
    while (*s == WHITESPACE)
        s++;
    return s;
}

char *clean_str(char *s){
    int i;
    /* TODO: Line should be in this format:
     *  OP arg1,arg2,arg3
     */

    /* Remove trailing whitespaces */
    s = trim_whitespaces(s);

    /* Remove unwanted characters */
    while (s[i]){

        /* If the character is a line break, cut the string */
        if (s[i] == '\n'){
            s[i] = '\0';
            break;
        }

        i++;
    }

    return s;
}

/*
 * Check if <s> starts with <t>
 */
bool starts_with(char *s, char *t){
    size_t len_s, len_t;

    len_s = strlen(s);
    len_t = strlen(t);

    return len_s < len_t ? false : strncmp(s, t, len_t) == 0;
}
