#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

/* Linked list's node structure */
struct Node{
    int tags;
    int data;
    int key;
    struct Node *next;
};

struct Node *node_create(int key, int data, int tags){
    struct Node *node;

    node = (struct Node *) calloc(1, sizeof(struct Node));

    node->data = data;
    node->key = key;
    node->tags = tags;
    node->next = NULL;

    return node;
}

/* Add a value at the end of a linked list */
/* Return the length of the list */
int l_list_add(struct Node *l_list, struct Node *new_node){
    int i;
    if (new_node == NULL || l_list == NULL)
        return 0;

    struct Node *last_node;

    last_node = (struct Node *) calloc(1, sizeof(struct Node));

    last_node = l_list;

    i = 2;

    while ( last_node->next != NULL ){
        last_node = last_node->next;
        i++;
    }

    last_node->next = new_node;

    return i;
}

void l_list_print(struct Node *l_list){
    printf("{");
    while ( l_list->next != NULL ){
        printf("\"%d\": \"%d\", ", l_list->key, l_list->data);
        l_list = l_list->next;
    }
    printf("\"%d\": \"%d\"}\n", l_list->key, l_list->data);
}

