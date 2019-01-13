#pragma once
#ifndef __CMAP_HAEDER_GUARD__
#define __CMAP_HAEDER_GUARD__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef int (*comparator_t)(const void*, const void*);
typedef int (*hasher_t)(const void*);
typedef void (*deleter_t)(void**);
typedef void (*process_t)(void*);
typedef void (*process_pair_t)(void*, void*);
typedef void** (*iterator_generator_t)(void*);
typedef void** (*iterator_stepper_t)(void**);
//抽象範囲型
struct vrange {
    void* object;
    void** (*begin)(void* object);
    void** (*end)(void* object);
    void** (*next)(void**);
};
typedef struct vrange* vrange_ptr;
void vrange_delete(vrange_ptr*);
void vrange_for(vrange_ptr, process_t);

//可変長配列
struct varray {
    size_t reserved, used;
    deleter_t deleter;
    void** memory;
};

typedef struct varray varray_t;
typedef struct varray* varray_ptr;

varray_ptr varray_new(size_t size, deleter_t deleter);
void varray_delete(varray_ptr*);
void varray_reserve(varray_ptr, size_t);
void varray_use(varray_ptr, size_t);
void varray_push(varray_ptr, void*);
void varray_pop(varray_ptr);
void* varray_top(varray_ptr);
void* varray_index(varray_ptr, size_t);
size_t varray_used(const varray_ptr);
void** varray_begin(varray_ptr);
void** varray_end(varray_ptr);
void** varray_next(void**);
void varray_for(varray_ptr, void (*process)(void*));
void varray_sort(varray_ptr, comparator_t);
void* varray_find(varray_ptr, const void*, comparator_t);     //線形探索
void* varray_find2(varray_ptr, const void*, comparator_t);    // 2分探索
int varray_find2_idx(varray_ptr, const void*, comparator_t);  // 2分探索
void varray_insert(varray_ptr,
                   void*,
                   comparator_t);  //ソートされたものに要素を追加
void varray_remove_idx(varray_ptr, int idx);
vrange_ptr vrange_create_varray(const varray_ptr);

//集合
struct vset {
    varray_ptr array;
    comparator_t comparator;
};
typedef struct vset vset_t;
typedef struct vset* vset_ptr;

vset_ptr vset_new(size_t reserve, deleter_t, comparator_t);
void vset_delete(vset_ptr* obj);
void vset_insert(vset_ptr, void*);
void vset_remove(vset_ptr, const void*);
bool vset_exist(vset_ptr, const void*);
void vset_for(vset_ptr, process_t);

//連想配列
struct vpair {
    void* key;
    void* value;
};
typedef struct vpair vpair_t;
typedef struct vpair* vpair_ptr;

struct vmap {
    size_t reserved, used;
    vpair_ptr pairs;
    comparator_t key_comparator;
    deleter_t key_deleter;
    deleter_t value_deleter;
};
typedef struct vmap vmap_t;
typedef struct vmap* vmap_ptr;

vmap_ptr vmap_new(size_t size,
                  comparator_t key_comp,
                  deleter_t key_del,
                  deleter_t value_del);
void vmap_delete(vmap_ptr*);
size_t vmap_used(const vmap_ptr);
void vmap_reserve(vmap_ptr,size_t);
int vmap_find_idx(const vmap_ptr,void* key);
void* vmap_find(const vmap_ptr,void* key);
bool vmap_exist(const vmap_ptr,void* key);
void vmap_insert(vmap_ptr,void* key,void*value);
void vmap_for(vmap_ptr,process_pair_t);


#endif