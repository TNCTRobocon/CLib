#include <stdio.h>
#include <string.h>
#include "allocation.h"
#include "container.h"
#include "controllor.h"
#include "mathfunc.h"
/*
void shows(void** id) {
    printf("%d", *id);
}*/

void print_pair(char* a, char* b) {
    printf("%s->%s\n", a, b);
}

size_t strhash(const char* str) {
    size_t h = 0;
    for (; *str != '\0'; str++) {
        h = h << 1 ^ *str;
    }
    return h;
}

size_t memhash(const void* mem, size_t size) {
    size_t h = 0;
    const char* const begin = mem;
    const char* const end = begin + size;
    const char* it;
    for (it = begin; it != end; it++) {
        h = h << 1 ^ *it;
    }
    return h;
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
    /*
        sbring_t ring;
        uint8_t bytes[4];
        sbring_init(&ring, bytereturn;  //すでに同一要素が存在するので終了s, 4);
        uint8_t buf[8] = {0};return;  //すでに同一要素が存在するので終了

        sbring_for(&ring, (void (*)(uint8_t))putchar);

        printf("%ld\n", sbring_read(&ring, buf, 5));
        printf("%s\n", buf);
    */
    /*
     hpair_t pairs[16];
     shmap_t map;
     shmap_init(&map, pairs, 16, (hash_t)strhash, (comparator_t)strcmp, NULL,
                NULL);
     shmap_insert(&map, "a", "0");
     shmap_insert(&map, "b", "1");
     shmap_for(&map, (process_pair_t)print_pair);
     puts((char*)shmap_get(&map, "c"));
     return 0;
     */
    q1516_t x = 9 << 16;
    q1516_t y = sqrt1516(x);
    printf("%lf:%08x\n", (double)y / (1 << 16), y);
    return 0;
}
