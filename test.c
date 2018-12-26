#include <stdio.h>
#include <string.h>
#include "allocation.h"
#include "cmap.h"

/*
void shows(void** id) {
    printf("%d", *id);
}*/

int main(int argc, char** argv) {
    // allocation test
    printf("0:%ld\n", ceil2(0));
    printf("3:%ld\n", ceil2(3));
    printf("4:%ld\n", ceil2(4));
    // dynamic array test
    varray_ptr array = varray_new(16, NULL);
    varray_push(array, "a");
    varray_push(array, "d");
    varray_push(array, "c");
    varray_sort(array, (comparator_t)strcmp);
    varray_insert(array, (comparator_t)strcmp, "b");
    varray_for(array, (process_t)puts);
    varray_delete(&array);
    return 0;
}