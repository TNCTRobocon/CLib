#pragma once
#ifndef __CONTROLLOR_HEADER_GUARD__
#define __CONTROLLOR_HEADER_GUARD__
#include <stdint.h>
#include <stddef.h>

typedef int32_t q1516_t;
typedef int64_t q3132_t;
struct spid{
    q1516_t p,i,d;
    q1516_t last_result;//前回の出力結果
    q1516_t last_error;//前回の誤差
    q1516_t sum_error;//累積エラー
};

typedef struct spid spid_t;
typedef struct spid* spid_ptr;

void spid_init(spid_ptr,q1516_t p,q1516_t i,q1516_t d);
void spid_deinit(spid_ptr);
q1516_t spid_process(spid_ptr, q1516_t target,q1516_t feedback);

struct svec{
    size_t size;
    q1516_t *array;
};
typedef struct svec svec_t;
typedef struct svec* svec_ptr;

void svec_init(svec_ptr , q1516_t* array, size_t size);
void svec_deinit(svec_ptr);
q3132_t svec_dot(svec_ptr,svec_ptr);

#endif