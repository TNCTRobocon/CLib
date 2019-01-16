#include <stdio.h>
#include <string.h>
#include "allocation.h"
#include "container.h"

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
    /*
    vmap_ptr map = vmap_new(16, (comparator_t)strcmp, NULL, NULL);
    printf("%ld\n", vmap_used(map));
    static char *aa = "a", *bb = "b", *cc = "c";
    vmap_insert(map, aa, "1");
    vmap_insert(map, bb, "2");
    vmap_insert(map, cc, "3");
    printf("%ld\n", vmap_used(map));
    vmap_for(map, (process_pair_t)print_pair);
    vmap_delete(&map);
    */

    bling_ptr ring = bling_new(4);
    uint8_t buf[8] = {0};
    printf("full=%ld,used=%ld,head=%ld,tail=%ld\n", ring->full, ring->used,
           ring->head, ring->tail);
    printf("%ld\n", bling_write(ring, (uint8_t*)"abcde", 5));
    printf("full=%ld,used=%ld,head=%ld,tail=%ld\n", ring->full, ring->used,
           ring->head, ring->tail);
    bling_for(ring, (void (*)(uint8_t))putchar);

    printf("%ld\n", bling_read(ring, buf, 5));
    printf("%s\n", buf);
    bling_delete(&ring);
    return 0;
}