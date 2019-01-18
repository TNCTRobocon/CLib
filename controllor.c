#include "controllor.h"

static inline int32_t clip32(int64_t x) {
    if (x > INT32_MAX) return INT32_MAX;
    if (x < INT32_MIN) return INT32_MIN;
    return x;
}

static inline size_t min(size_t a, size_t b) {
    return a < b ? a : b;
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
