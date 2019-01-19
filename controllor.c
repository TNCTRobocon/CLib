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
    const q1516_t eps = 1 << 2;
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
    vec->size = array ? size : 0;
}

void svec_deinit(svec_ptr vec) {
    //何もしない
}

q3132_t svec_dot(const svec_ptr a, const svec_ptr b) {
    if (!a || !b) return 0;
    size_t size = min(a->size, b->size), idx;
    q3132_t sum = 0;
    const q1516_t *aa = a->array, *bb = b->array;
    for (idx = 0; idx < size; idx++) {
        sum += (q3132_t)aa[idx] * bb[idx];
    }
    return sum;
}

svec_ptr svec_add(svec_ptr dest, const svec_ptr a, const svec_ptr b) {
    if (!dest || !a || !b) return NULL;
    const q1516_t *aa = a->array, *bb = b->array;
    q1516_t* dd = dest->array;
    size_t size = min3(a->size, b->size, dest->size), idx;
    for (idx = 0; idx < size; idx++) {
        dd[idx] = aa[idx] + bb[idx];
    }
    return dest;
}

svec_ptr svec_sub(svec_ptr dest, const svec_ptr a, const svec_ptr b) {
    if (!dest || !a || !b) return NULL;
    const q1516_t *aa = a->array, *bb = b->array;
    q1516_t* dd = dest->array;
    size_t size = min3(a->size, b->size, dest->size), idx;
    for (idx = 0; idx < size; idx++) {
        dd[idx] = aa[idx] - bb[idx];
    }
    return dest;
}

svec_ptr svec_neg(svec_ptr dest, const svec_ptr inst) {
    if (!dest || !inst) return NULL;
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

q1516_t svec_len2(const svec_ptr inst) {
    if (!inst) return 0;
    q3132_t sum = 0;
    size_t idx;
    for (idx = 0; idx < inst->size; idx++) {
        sum += inst->array[idx];
    }
    return clip32(sum >> 16);
}

q1516_t svec_len(const svec_ptr inst) {
    return sqrt1516(svec_len2(inst));
}

svec_ptr svec_normalize(svec_ptr inst, const svec_ptr dest) {
    if (!inst || !dest) return NULL;
    q1516_t len = svec_len(inst);
    size_t size = min(inst->size, dest->size), idx;
    for (idx = 0; idx < size; idx++) {
        dest->array[idx] = ((q3132_t)inst->array << 16) / len;
    }
    return dest;
}

svec_ptr svec_cmul(svec_ptr dest, const svec_ptr inst, q3132_t val) {
    if (!inst || !dest) return NULL;
    size_t size = min(inst->size, dest->size), idx;
    for (idx = 0; idx < size; idx++) {
        dest->array[idx] = clip32(((q3132_t)inst->array * val) >> 16);
    }
    return dest;
}

void smat_init(smat_ptr obj, q1516_t* mat, size_t x, size_t y) {
    if (!obj) return;
    obj->mat = mat;
    obj->x = mat ? x : 0;
    obj->y = mat ? y : 0;
}

void smat_deinit(smat_ptr obj) {}

smat_ptr smat_set(smat_ptr mat, q1516_t val) {
    if (!mat) return NULL;
    q1516_t* const begin = mat->mat;
    q1516_t* const end = mat->mat + mat->x * mat->y;
    q1516_t* it;
    for (it = begin; it != end; it++) {
        *it = val;
    }
    return mat;
}

bool smat_is_match(const smat_ptr a, const smat_ptr b) {
    if (!a || !b) return false;
    return a->x == b->x && a->y == b->y;
}

bool smat_is_square(const smat_ptr a) {
    return a ? a->x == a->y : false;
}

smat_ptr smat_unit(smat_ptr d, q1516_t val) {
    if (!d) return NULL;
    q1516_t* dd = d->mat;
    size_t i, j, p;
    for (j = 0, p = 0; j < d->y; j++)
        for (i = 0; i < d->x; i++, p++) {
            dd[p] = i != j ? 0 : val;
        }
    return d;
}

q1516_t smat_idx(const smat_ptr mat, size_t x, size_t y) {
    if (!mat) return 0;
    const size_t p = x + mat->x * y;
    const size_t size = mat->x * mat->y;
    return p < size ? mat->mat[p] : 0;
}

smat_ptr smat_add(smat_ptr d, const smat_ptr a, const smat_ptr b) {
    // Nullチェック
    if (!d || !a || !b) return NULL;
    //型チェック
    if ((d->x != a->x) || (d->x != b->x) || (d->y != a->y) || (d->y != b->y))
        return NULL;
    //計算
    size_t pos, size = d->x * d->y;
    for (pos = 0; pos < size; pos++) {
        d->mat[pos] = a->mat[pos] + b->mat[pos];
    }
    return d;
}

smat_ptr smat_sub(smat_ptr d, const smat_ptr a, const smat_ptr b) {
    // Nullチェック
    if (!d || !a || !b) return NULL;
    //型チェック
    if ((d->x != a->x) || (d->x != b->x) || (d->y != a->y) || (d->y != b->y)) {
        return NULL;
    }
    //計算
    size_t pos, size = d->x * d->y;
    for (pos = 0; pos < size; pos++) {
        d->mat[pos] = a->mat[pos] - b->mat[pos];
    }
    return d;
}

smat_ptr smat_cmul(smat_ptr d, const smat_ptr a, q1516_t c) {
    // Nullチェック
    if (!d || !a) return NULL;
    //型チェック
    if ((d->x != a->x) || (d->y != a->y)) return NULL;
    //計算
    size_t pos, size = d->x * d->y;
    for (pos = 0; pos < size; pos++) {
        d->mat[pos] = ((q3132_t)a->mat[pos] * c) >> 16;
    }
    return d;
}

svec_ptr smat_vmul(svec_ptr y, const smat_ptr A, const svec_ptr x) {
    // Null チェック
    if (!y || !A || !x) return NULL;
    // 型チェック
    if ((A->x != x->size) || (A->y != y->size)) return NULL;
    //計算
    size_t i, j, p;
    for (j = 0, p = 0; j < A->y; j++) {
        q3132_t sum = 0;
        for (i = 0; i < A->x; i++, p++) {
            sum += (q3132_t)A->mat[p] * x->array[i];
        }
        y->array[j] = clip32(sum >> 16);
    }
    return y;
}

smat_ptr smat_mul(smat_ptr d, const smat_ptr a, const smat_ptr b) {
    // Null チェック
    if (!a || !b || !d) return NULL;
    // 型チェック
    bool cl = a->y == d->y, cm = a->x == b->y, cn = b->x == d->x;
    if (!cl || !cm || !cn) return NULL;
    //計算
    size_t i, j, k, p;
    for (i = 0, p = 0; i < d->x; i++)
        for (j = 0; j < d->y; j++, p++) {
            q3132_t sum = 0;
            for (k = 0; k < a->x; k++) {
                sum += (q3132_t)a->mat[(i + k) + j * a->x] *
                       b->mat[i + (j + k) * b->x];
            }
            d->mat[p] = clip32(sum >> 16);
        }
    return d;
}

smat_ptr smat_cdiv(smat_ptr d, const smat_ptr a, q1516_t c) {
    // Null チェック
    if (!d && !a && !c) return NULL;
    //型チェック
    if ((d->x != a->x) || (d->y != a->y)) return NULL;
    //計算
    size_t pos, size = d->x * d->y;
    for (pos = 0; pos < size; pos++) {
        d->mat[pos] = (q3132_t)a->mat[pos] << 16 / c;
    }
    return d;
}

smat_ptr smat_tri(smat_ptr d, const smat_ptr a) {
    if (!a || smat_is_square(a)) return NULL;
    //三角行列を生成する
    const size_t n = a->x;
    size_t i, j, k;
    q1516_t per;
    const q1516_t* aa = a->mat;
    q1516_t* dd = d->mat;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            if (i < j) {
                per = ((q3132_t)aa[j * n + i] << 16 / aa[i * n + i]);
                for (k = 0; k < n; k++) {
                    dd[j * n + k] -= (q3132_t)aa[i * n + k] * per >> 16;
                }
            }
        }
    return d;
}

q1516_t smat_det(smat_ptr buf, const smat_ptr a) {
    if (!smat_tri(buf, a)) return 0;
    size_t idx, n = a->x;
    q1516_t det = 1 << 16;
    const q1516_t* aa = a->mat;
    for (idx = 0; idx < n; idx++) {
        det = ((q3132_t)det * aa[idx * n + idx]) >> 16;
    }
    return det;
}

smat_ptr smat_inv2(smat_ptr d, const smat_ptr a) {
    if (!d || !a) return NULL;
    //型チェック
    if (!smat_is_square(a) || !smat_is_square(d)) return NULL;
    if (a->x != 2 || d->x != 2) return NULL;
    //計算
    const q1516_t* aa = a->mat;
    q1516_t* dd = d->mat;
    q1516_t det = dd[0] * dd[3] - dd[1] * dd[2];
    dd[0] = aa[3] << 16;
    dd[1] = -aa[2] << 16;
    dd[2] = -aa[1] << 16;
    dd[3] = aa[0] << 16;
    return smat_cdiv(d, d, det);
}
