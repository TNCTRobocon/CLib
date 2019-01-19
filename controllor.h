#ifndef __CONTROLLOR_HEADER_GUARD__
#define __CONTROLLOR_HEADER_GUARD__
#include "types.h"

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


#endif