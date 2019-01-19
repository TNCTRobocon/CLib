#include "controllor.h"
#include <stdio.h>

static inline int32_t clip32(int64_t x) {
    if (x > INT32_MAX) return INT32_MAX;
    if (x < INT32_MIN) return INT32_MIN;
    return x;
}

static inline size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}
static inline size_t min3(size_t a, size_t b, size_t c) {
    return min(min(a, b), c);
}

static inline int32_t abs32(int32_t a) {
    return a > 0 ? a : -a;
}

q1516_t sqrt1516(q1516_t x) {
    // newtown方による解法
    const q1516_t eps = 1<<2;
    const size_t limit = 8;  //試行回数の制限
    size_t cnt;
    q1516_t now = 1 << 16, next = 0;
    q3132_t f;
    q1516_t g;
    for (cnt = 0; cnt < limit; cnt++) {
        f = (((q3132_t)now * now) >> 16) - x;
        g = 2 * now;
        next = now - ((f << 16) / g);
        if (abs32(now - next) < eps) {
            return next;
        } else {
            now = next;
        }
    }
    return next;
}

void spid_init(spid_ptr pid, q1516_t p, q1516_t i, q1516_t d) {
    if (!pid) return;
    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->last_error = 0;
    pid->last_result = 0;
    pid->sum_error = 0;
}

void spid_deinit(spid_ptr pid) {
    //特に何もしない
}

q1516_t spid_process(spid_ptr pid, q1516_t target, q1516_t feedback) {
    if (!pid) return 0;
    //準備
    q1516_t error = feedback - target;
    q1516_t sum_error = pid->sum_error + error;
    q1516_t diff_error = error - pid->last_error;
    //計算開始
    q3132_t result = pid->last_result;
    result += (q3132_t)pid->p * error;
    result += (q3132_t)pid->i * sum_error;
    result += (q3132_t)pid->d * diff_error;
    q1516_t clip_result = clip32(result);
    //更新
    pid->last_result = clip_result;
    pid->last_error = error;
    pid->sum_error = sum_error;
    return clip_result;
}

void svec_init(svec_ptr vec, q1516_t* array, size_t size) {
    if (!vec && !vec) return;
    vec->array = array;
    vec->size = size;
}

void svec_deinit(svec_ptr vec) {
    //何もしない
}

q3132_t svec_dot(svec_ptr a, svec_ptr b) {
    if (!a && !b) return 0;
    size_t size = min(a->size, b->size), idx;
    q3132_t sum = 0;
    const q1516_t *aa = a->array, *bb = b->array;
    for (idx = 0; idx < size; idx++) {
        sum += (q3132_t)aa[idx] * bb[idx];
    }
    return sum;
}

svec_ptr svec_add(svec_ptr dest, const svec_ptr a, const svec_ptr b) {
    if (!dest && !a && !b) return NULL;
    const q1516_t *aa = a->array, *bb = b->array;
    q1516_t* dd = dest->array;
    size_t size = min3(a->size, b->size, dest->size), idx;
    for (idx = 0; idx < size; idx++) {
        dd[idx] = aa[idx] + bb[idx];
    }
    return dest;
}

svec_ptr svec_sub(svec_ptr dest, const svec_ptr a, const svec_ptr b) {
    if (!dest && !a && !b) return NULL;
    const q1516_t *aa = a->array, *bb = b->array;
    q1516_t* dd = dest->array;
    size_t size = min3(a->size, b->size, dest->size), idx;
    for (idx = 0; idx < size; idx++) {
        dd[idx] = aa[idx] - bb[idx];
    }
    return dest;
}

svec_ptr svec_neg(svec_ptr dest, const svec_ptr inst) {
    if (!dest && !inst) return NULL;
    const q1516_t* ii = inst->array;
    q1516_t* dd = dest->array;
    size_t size = min(dest->size, inst->size), idx;
    for (idx = 0; idx < size; idx++) {
        dd[idx] = -ii[idx];
    }
    return NULL;
}

svec_ptr svec_set(svec_ptr dest, q1516_t val) {
    if (!dest) return NULL;
    size_t idx;
    for (idx = 0; idx < dest->size; idx++) {
        dest->array[idx] = val;
    }
    return dest;
}

q1516_t svec_idx(const svec_ptr inst, size_t idx) {
    if (!inst) return 0;
    return idx < inst->size ? inst->array[idx] : 0;
}

q1516_t svec_length2(svec_ptr dest, const svec_ptr inst) {
    if (!dest) return 0;
    q3132_t sum = 0;
    size_t idx, size = min(dest->size, inst->size);
    for (idx = 0; idx < size; idx++) {
        sum += inst->array[idx];
    }
    return clip32(sum >> 16);
}
