#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdbool.h>

bool contain_label(char *);
bool relevant_line(char *s);
void first_pass(char *fname, struct Node *external_labels_ptr);

#endif
