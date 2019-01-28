#include "mathfunc.h"
static inline int32_t abs32(int32_t a) {
    return a > 0 ? a : -a;
}

static inline int64_t abs64(int64_t a) {
    return a > 0 ? a : -a;
}

q1516_t sqrt1516(q1516_t x) {
    // newton方による解法
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

q1516_t exp1516(q1516_t x) {
    //テイラー展開
    const size_t limit = 12;  //試行制限
    const q3132_t eps = 1 << 16;
    size_t cnt;
    int32_t fact = 1;
    //第一項と第二項の計算
    q3132_t pow = (q3132_t)x << 16;
    q3132_t now = pow + (1l << 32), diff;
    //第n項の計算
    for (cnt = 2; cnt < limit; cnt++) {
        fact *= cnt;
        pow = (pow * x) >> 16;
        now += diff = pow / fact;
        if (abs64(diff) < eps) {
            return now >> 16;
        }
    }
    return now >> 16;
}
