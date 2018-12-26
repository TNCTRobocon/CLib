#include <stdio.h>
#include "allocation.h"
#include "cmap.h"

void show(void* id) {
    printf("%s", (char*)id);
}
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
    varray_push(array, "hello");
    varray_push(array, "world");
    vrange_ptr range = vrange_create_varray(array);
    vrange_for(range, show);

    varray_pop(array);
    varray_for_each(array, show);
    varray_delete(&array);
    return 0;
}