#include <stdio.h>
#include "allocation.h"
int main(int argc, char** argv) {
    // allocation test
    printf("0:%ld\n", ceil2(0));
    printf("3:%ld\n", ceil2(3));
    printf("4:%ld\n", ceil2(4));
    return 0;
}