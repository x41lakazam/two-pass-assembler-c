#include <stdlib.h>
#include "linked_list.c"

int main(int argc, char *argv[]){
    struct Node l_list;
    int i;

    l_list = l_list_create(2, 1);

    l_list_add(&l_list, 4, 2);
    l_list_add(&l_list, 10, 3);
    i = l_list_add(&l_list, 19, 4);

    l_list_print(&l_list);

    return 1;
}
