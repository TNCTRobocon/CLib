#include <stdio.h>
#include <string.h>
#include "allocation.h"
#include "ccontainer.h"

/*
void shows(void** id) {
    printf("%d", *id);
}*/

void print_pair(char* a, char* b) {
    printf("%s->%s\n", a, b);
}

int main(int argc, char** argv) {
    /*// allocation test
    printf("0:%ld\n", ceil2(0));
    printf("3:%ld\n", ceil2(3));
    printf("4:%ld\n", ceil2(4));
    // dynamic array test
    varray_ptr array = varray_new(16, NULL);
    varray_push(array, "a");
    varray_push(array, "d");
    varray_push(array, "c");
    varray_sort(array, (comparator_t)strcmp);
    varray_insert(array, "b", (comparator_t)strcmp);
    varray_for(array, (process_t)puts);
    varray_delete(&array);*/
    // set
    /*
    vset_ptr set = vset_new(8, NULL, (comparator_t)strcmp);
    vset_insert(set, "a");
    vset_insert(set, "a");
    vset_insert(set, "b");
    vset_for(set, (process_t)puts);
    printf("%d\n", vset_exist(set, "a"));
    puts("---");
    vset_remove(set, "a");
    printf("%d\n", vset_exist(set, "a"));
    vset_for(set, (process_t)puts);
    */
    vmap_ptr map = vmap_new(16, (comparator_t)strcmp, NULL, NULL);
    printf("%d\n", vmap_used(map));
    vmap_insert(map, "a", "1");
    vmap_insert(map, "b", "2");
    vmap_insert(map, "c", "3");
    printf("%d\n", vmap_used(map));
    vmap_for_pair(map, (process_pair_t)print_pair);

    return 0;
}