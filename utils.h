#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

char *trim_whitespaces(char *);

bool starts_with(char *, char *);

char *clean_str(char *s);

char *get_entries_outfile(char *filename);
char *get_externals_outfile(char *filename);
char *get_basename(char *filename);

#endif
