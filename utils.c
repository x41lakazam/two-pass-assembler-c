/*
 * Utilitaries functions
 */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "globals.h"

int get_line_wout_spaces(char **buffer, size_t *size, FILE *file){
    int    c;
    size_t count;
    int space_seen;
    char *new_lineptr;

    if (buffer == NULL)
        return 0;

    space_seen = 0;
	count = 0;

	while ((c = getc(file)) != EOF && (c != '\n')){
		/* If c is a space */
		if (isspace(c)){
			/* If a space has already been seen, ignore this one */
			if (space_seen == 1)
				continue;
			/* Else set space_seen to 1 */
			space_seen = 1;
		}
		else
			space_seen = 0;
		(*buffer)[count++] = (char)c;

		if (count + 1 >= *size){
            /* Reallocate space (double the size) */
            new_lineptr = (char *) realloc(*buffer, 2 * *size);
            if (new_lineptr == NULL){
                printf("Internal error: Cannot parse line, too long.\n");
                break;
            }
            *buffer = new_lineptr;
            *size *= 2;
        }
	}

    (*buffer)[count] = '\0';
	if (count == 0)
		return -1;
    return count;
}

char *get_basename(char *fname){
    return strtok(fname, ".");
}

char *trim_whitespaces(char *s){
    while (*s == WHITESPACE)
        s++;
    return s;
}

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
		clean_s[j++] = s[i++]; /* Add the colon */
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

bool starts_with(char *s, char *t){
    size_t len_s, len_t;

    len_s = strlen(s);
    len_t = strlen(t);

    return len_s < len_t ? false : strncmp(s, t, len_t) == 0;
}
