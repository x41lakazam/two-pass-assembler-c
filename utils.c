/*
 * Utilitaries functions
 */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "globals.h"

char *get_basename(char *fname){
    return strtok(fname, ".");
}

/*
 * Remove every leading whitespace
 */
char *trim_whitespaces(char *s){
    while (*s == WHITESPACE)
        s++;
    return s;
}

/*
 * Clean a string:
 * remove trailing whitespaces/unwanted chars
 * remove extra whitespaces
 */
char *clean_str(char *s){
    int i, j;
    char *clean_s; /* Hold the new and clean string */
    int first_whitespace = 1; /* Flag - true if no whitespace have been seen yet */

    clean_s = (char *) calloc(LINE_MAX_SIZE, sizeof(char));

    /* Remove trailing whitespaces */
    s = trim_whitespaces(s);

    i = j = 0;

	/* If the string contain a label, remove every whitespace until the operation name */
	if ( strchr(s, ':') != NULL){
		/* Go to the colon and ignore every whitespace */
		while(s[i] != ':'){
			if (s[i] != ' ')
				clean_s[j++] = s[i];
			i++;
		}
		i++; /* Skip the colon itself */
		/* Remove every whitespace until the operation */
		while (isspace(s[i])){
			i++;
		}
	}

    /* Remove extra whitespaces (keep only the first one) and linebreak */
    while (s[i]){

		/* If the char is a double quote, add everything until the corresponding quote */
		if (s[i] == '"'){

			clean_s[j++] = s[i++]; /* Add the first quote */

			while (s[i] != '"')
				clean_s[j++] = s[i++]; /* Add the text in between */

			clean_s[j++] = s[i++]; /* Add the second quote */
		}

        /* If the char is a whitespace:
         * if it's the first one then add it, else ignore it */
        if (s[i] == ' '){
            if ( first_whitespace ){
                first_whitespace = 0;
                clean_s[j++] = s[i];
            }
            i++;
        }

        /* If the char is a linebreak, don't add it */
        else if (s[i] == '\n')
            break;

        /* If the char is a wanted char, simply add it */
        else
            clean_s[j++] = s[i++];
    }

    clean_s[j] = '\0';
    return clean_s;
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
