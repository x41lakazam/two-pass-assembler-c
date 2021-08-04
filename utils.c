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

/*
 * Check if <s> starts with <t>
 */
bool starts_with(char *s, char *t){
    size_t len_s, len_t;

    len_s = strlen(s);
    len_t = strlen(t);

    return len_s < len_t ? false : strncmp(s, t, len_t) == 0;
}
