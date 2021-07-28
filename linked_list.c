#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

/* Linked list's node structure */
struct Node{
    int data;
    int key;
    struct Node *next;
};

struct Node l_list_create(int data, int key){
    struct Node node;

    node.data = data;
    node.key = key;
    node.next = NULL;

    return node;
}

/* Add a value at the end of a linked list */
/* Return the length of the list */
int l_list_add(struct Node *l_list, int data, int key){
    int i;
    struct Node *new_node;
    struct Node *last_node;

    new_node = (struct Node *) calloc(1, sizeof(struct Node));
    last_node = (struct Node *) calloc(1, sizeof(struct Node));

    new_node->data = data;
    new_node->key = key;
    new_node->next = NULL;

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

