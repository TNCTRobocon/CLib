#pragma once
#ifndef __ALLOCATION_HEADER_GUARD__
#define __ALLOCATION_HEADER_GUARD__

#include <stddef.h>
#include <stdint.h>

static inline uint32_t count32(uint32_t v) {
    uint32_t count = (v & 0x55555555) + ((v >> 1) & 0x55555555);
    count = (count & 0x33333333) + ((count >> 2) & 0x33333333);
    count = (count & 0x0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f);
    count = (count & 0x00ff00ff) + ((count >> 8) & 0x00ff00ff);
    return (count & 0x0000ffff) + ((count >> 16) & 0x0000ffff);
}

static inline uint32_t msb32(uint32_t n) {
    if (n == 0)
        return 0;
    // msbを計算する
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    return count32(n)-1;
}

static inline size_t ceil2(size_t n) {
    // 2のべき乗に調整する
    if (!n){
        return 0;
    }else if (n & (n - 1)) {
        return 1<<(msb32(n-1)+1);
    } else {
        return n;
    }
}

#endif