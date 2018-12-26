#include "cmap.h"
#include <stdlib.h>
#include <string.h>
#include "allocation.h"

#define SWAP(type, a, b) \
    do {                 \
        type _c;         \
        _c = a;          \
        a = b;           \
        b = _c;          \
    } while (0)
static const size_t mininum = 8;
static inline size_t max(size_t a, size_t b) {
    return a > b ? a : b;
}

void vrange_delete(vrange_ptr* range) {
    free(*range);
    range = NULL;
}

void vrange_for(vrange_ptr range, process_t process) {
    void** const begin = range->begin(range->object);
    void** const end = range->end(range->object);
    void** it;
    for (it = begin; it != end; it = range->next(it)) {
        process(*it);
    }
}

varray_ptr varray_new(size_t size, deleter_t deleter) {
    //先に確保するメモリを計算する
    size_t reserve = max(ceil2(size), mininum);
    //メモリ確保
    void** mem = (void**)malloc(reserve * sizeof(void*));
    if (!mem) {
        return NULL;
    }
    varray_ptr obj = (varray_ptr)malloc(sizeof(varray_t));
    if (!obj) {
        free(mem);
        return NULL;
    }
    //格納
    obj->memory = mem;
    obj->reserved = reserve;
    obj->deleter = deleter;
    obj->used = 0;
    return obj;
}

void varray_delete(varray_ptr* ref) {
    // NULL Pointer
    if (!ref && !*ref) {
        return;
    }
    varray_ptr obj = *ref;
    //個々の要素の削除
    if (obj->deleter) {
        void** const start = &obj->memory[0];
        void** const end = &obj->memory[obj->used];
        void** it;
        for (it = start; it != end; it++) {
            obj->deleter(it);
        }
    }
    //本体の削除
    free(obj->memory);
    free(obj);
    *ref = NULL;
}

void varray_reserve(varray_ptr obj, size_t size) {
    if (!obj) return;
    //メモリの確保が必要か?
    if (size > obj->reserved) {
        size_t reserve = ceil2(size);
        obj->memory = realloc(obj->memory, reserve);
        obj->reserved = reserve;
    }
}

void varray_use(varray_ptr obj, size_t size) {
    if (!obj) return;
    varray_reserve(obj, size);
    obj->used = size;
}

void varray_push(varray_ptr obj, void* newer) {
    if (!obj) return;
    varray_reserve(obj, obj->used + 1);
    obj->memory[obj->used++] = newer;
}

void varray_pop(varray_ptr obj) {
    //前提条件確認
    if (!obj || obj->used == 0) {
        return;
    }

    if (obj->deleter) {
        obj->deleter(&obj->memory[obj->used]);
    }
    obj->used--;
}

void* varray_top(varray_ptr obj) {
    return obj ? obj->memory[obj->used] : NULL;
}

void* varray_index(varray_ptr obj, size_t idx) {
    if (!obj) {
        return NULL;
    } else {
        return idx < obj->used ? obj->memory[idx] : NULL;
    }
}

void** varray_begin(varray_ptr obj) {
    return obj ? &obj->memory[0] : NULL;
}
void** varray_end(varray_ptr obj) {
    return obj ? &obj->memory[obj->used] : NULL;
}

void** varray_next(void** obj) {
    return obj ? obj + 1 : NULL;
}

void varray_for(varray_ptr obj, void (*process)(void*)) {
    if (!obj) return;
    void** const begin = obj->memory;
    void** const end = obj->memory + obj->used;
    void** it;
    for (it = begin; it != end; it++) {
        process(*it);
    }
}

void varray_sort(varray_ptr obj, comparator_t comp) {
    if (!obj || !comp) return;
    //適当に選択ソート 必要があれば真面目に書く
    void** const begin = obj->memory;
    void** const end = obj->memory + obj->used;
    void **it, **jt;
    void** select;
    for (it = begin; it != end; it++) {
        select = it;
        for (jt = it + 1; jt != end; jt++) {
            if (comp(*select, *jt) > 0) {
                select = jt;
            }
        }
        SWAP(void*, *it, *select);
    }
}

void* varray_find(varray_ptr obj, const void* key, comparator_t comp) {
    if (!obj || !comp) return NULL;
    void** const begin = obj->memory;
    void** const end = obj->memory + obj->used;
    void** it;
    for (it = begin; it != end; it++) {
        if (!comp(*it, key)) {
            return *it;
        }
    }
    return NULL;
}

void* varray_find2(varray_ptr obj, const void* key, comparator_t comp) {
    int idx = varray_find2_idx(obj, key, comp);
    return idx >= 0 ? obj->memory[idx] : NULL;
}
int varray_find2_idx(varray_ptr obj, const void* key, comparator_t comp) {
    if (!obj || !comp) return -1;
    void** list = obj->memory;
    int begin = 0, end = obj->used, mid;
    while (begin < end) {
        mid = begin + ((end - begin) >> 1);
        int branch = comp(list[mid], key);
        if (!branch) {
            return mid;
        } else if (branch > 0) {
            end = mid - 1;
        } else {
            begin = mid + 1;
        }
    }
    return -1;
}

void varray_insert(varray_ptr obj, void* item, comparator_t comp) {
    if (!obj || !comp) return;
    void** const begin = obj->memory;
    void** const end = obj->memory + obj->used;
    void **it, **jt;
    varray_reserve(obj, obj->used + 1);
    //場所を確認(手抜き)
    for (it = begin; it != end; it++) {
        if (comp(*it, item) > 0) break;
    }
    //ずらす
    for (jt = end; jt != it; jt--) {
        *jt = *(jt - 1);
    }
    *it = item;
    obj->used++;
}

void varray_remove_idx(varray_ptr obj, int idx) {
    if (!obj || idx < 0 || idx >= obj->used) return;
    void** mem = obj->memory;
    size_t end = obj->used;
    size_t it;
    for (it = idx; it < end - 1; it++) {
        mem[it] = mem[it + 1];
    }
    obj->used--;
}

vrange_ptr vrange_create_varray(const varray_ptr obj) {
    if (!obj) return NULL;
    vrange_ptr range = malloc(sizeof(varray_t));
    if (!range) return NULL;
    range->object = obj;
    range->begin = (iterator_generator_t)varray_begin;
    range->end = (iterator_generator_t)varray_end;
    range->next = (iterator_stepper_t)varray_next;
    return range;
}

vset_ptr vset_new(size_t reserve, deleter_t del, comparator_t comp) {
    if (!comp) return NULL;
    varray_ptr array = varray_new(reserve, del);
    if (!array) return NULL;
    vset_ptr set = (vset_ptr)malloc(sizeof(vset_t));
    if (!set) {
        varray_delete(&array);
    }
    set->array = array;
    set->comparator = comp;
    return set;
}

void vset_delete(vset_ptr* ref) {
    if (!ref && !*ref) return;
    vset_ptr obj = *ref;
    varray_delete(&(obj->array));
    free(obj);
    *ref = NULL;
}

void vset_insert(vset_ptr obj, void* item) {
    if (!obj) return;
    comparator_t comp = obj->comparator;
    varray_ptr array = obj->array;
    if (varray_find2(array, item, comp) == NULL) {
        varray_insert(array, item, comp);
    }
}

bool vset_exist(vset_ptr obj, const void* cmp) {
    if (!obj) return false;
    comparator_t comp = obj->comparator;
    varray_ptr array = obj->array;
    return varray_find2(array, cmp, comp) != NULL;
}

void vset_remove(vset_ptr obj, const void* cmp) {
    if (!obj) return;
    int idx = varray_find2_idx(obj->array, cmp, obj->comparator);
    varray_remove_idx(obj->array, idx);
}

void vset_for(vset_ptr obj, process_t process) {
    if (!obj) return;
    varray_for(obj->array, process);
}

vmap_ptr vmap_new(size_t size,
                  deleter_t key_del,
                  comparator_t key_comp,
                  deleter_t value_del) {
    varray_ptr array = varray_new(size, NULL);
    if (!array) return NULL;
    vmap_ptr map = (vmap_ptr)malloc(sizeof(vmap_t));
    if (!map) {
        varray_delete(&array);
        return NULL;
    }
    map->array = array;
    map->key_deleter = key_del;
    map->key_comparator = key_comp;
    map->value_deleter = value_del;
    return map;
}

void* vmap_find(vmap_ptr obj, void* key) {
    return varray_find2(obj->array, key, obj->key_comparator);
}

bool vmap_exist(vmap_ptr obj, void* key) {
    return varray_find2_idx(obj->array, key, obj->key_comparator) >= 0;
}

void vmap_insert(vmap_ptr obj, void* key, void* value) {
    if (!obj) return;
    if (!vmap_find(obj, key)) return;
    vpair_ptr pair = (vpair_ptr)malloc(sizeof(vpair_t));
    if (!pair) return;
    pair->key = key;
    pair->value = value;
    varray_insert(obj->array, pair, obj->key_comparator);
}

void vmap_remove(vmap_ptr obj, void* key) {
    if (!obj) return;
    int idx = varray_find2_idx(obj->array, key, obj->key_comparator);
    varray_remove_idx(obj->array, idx);
}