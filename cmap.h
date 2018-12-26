#pragma once
#ifndef __CMAP_HAEDER_GUARD__
#define __CMAP_HAEDER_GUARD__
#include <stddef.h>
#include <stdint.h>

typedef int (*comparator_t)(const void*, const void*);
typedef int (*hasher_t)(const void*);
typedef void (*deleter_t)(void**);
typedef void (*process_t)(void*);
typedef void** (*iterator_generator_t)(void*);
typedef void** (*iterator_stepper_t)(void**);
//抽象範囲型
struct vrange{
    void* object;
    void** (*begin)(void* object);
    void** (*end)(void* object);
    void** (*next)(void**);
};
typedef struct vrange *vrange_ptr;
void vrange_delete(vrange_ptr*);
void vrange_for(vrange_ptr,process_t);

//可変長配列
struct varray {
    size_t reserved, used;
    deleter_t deleter;
    void** memory;
};

typedef struct varray varray_t;
typedef struct varray *varray_ptr;

varray_ptr varray_new(size_t size, deleter_t deleter);
void varray_delete(varray_ptr*);
void varray_push(varray_ptr, void*);
void varray_pop(varray_ptr);
void* varray_top(varray_ptr);
void* varray_index(varray_ptr, size_t);
size_t varray_used();
void** varray_begin(varray_ptr);
void** varray_end(varray_ptr);
void** varray_next(void**);
void varray_for(varray_ptr,void(*process)(void*));
void varray_sort(varray_ptr,comparator_t);
vrange_ptr vrange_create_varray(const varray_ptr);

//集合
struct vset {
    varray_t array;
    comparator_t comparator;
};

//連想配列
struct vpair {
    void *first, *second;
};
typedef struct vpair vpair_t;
struct vmap {
    size_t reserved, used;
    comparator_t comparator;
    deleter_t deleter;
    vpair_t* array;
};

#endif