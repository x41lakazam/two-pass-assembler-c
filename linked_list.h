#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* Linked list's node structure */
struct Node;

struct Node *node_create(int key, int data, int tags);
int l_list_add(struct Node *l_list, struct Node *new_node);
void l_list_print(struct Node *l_list);

#endif
