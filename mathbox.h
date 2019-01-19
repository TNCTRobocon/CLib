#ifndef __MATHBOX_HEADER_GUARD__
#define __MATHBOX_HEADER_GUARD__

#include "types.h"



struct svec{
    size_t size;
    q1516_t *array;
};
typedef struct svec svec_t;
typedef struct svec* svec_ptr;

void svec_init(svec_ptr , q1516_t* array, size_t size);
void svec_deinit(svec_ptr);
q3132_t svec_dot(const svec_ptr,const svec_ptr);
svec_ptr svec_add(svec_ptr,const svec_ptr,const svec_ptr);
svec_ptr svec_sub(svec_ptr,const svec_ptr,const svec_ptr);
svec_ptr svec_neg(svec_ptr,const svec_ptr);
svec_ptr svec_set(svec_ptr,q1516_t);
q1516_t svec_idx(const svec_ptr,size_t);
q1516_t svec_len2(const svec_ptr);//|x|^2
q1516_t svec_len(const svec_ptr);//|x|
svec_ptr svec_normalize(svec_ptr,const svec_ptr);//x/|x|
svec_ptr svec_cmul(svec_ptr,const svec_ptr,q3132_t);

struct smat{
    size_t x,y;
    q1516_t *mat;
};

typedef struct smat smat_t;
typedef struct smat* smat_ptr;
void smat_init(smat_ptr obj,q1516_t*mat,size_t x,size_t y);
void smat_deinit(smat_ptr obj);
bool smat_is_match(const smat_ptr,const smat_ptr);//大きさが等しいか
bool smat_is_square(const smat_ptr);


smat_ptr smat_set(smat_ptr,q1516_t);
smat_ptr smat_unit(smat_ptr,q1516_t);
q1516_t smat_idx(const smat_ptr,size_t x,size_t y);//安全
smat_ptr smat_add(smat_ptr,const smat_ptr,const smat_ptr);
smat_ptr smat_sub(smat_ptr,const smat_ptr,const smat_ptr);
smat_ptr smat_cmul(smat_ptr,const smat_ptr,q1516_t);
svec_ptr smat_vmul(svec_ptr y,const smat_ptr A,const svec_ptr x);//Ax'=y'
smat_ptr smat_mul(smat_ptr d,const smat_ptr a,const smat_ptr b);//AB=D

smat_ptr smat_cdiv(smat_ptr,const smat_ptr,q1516_t);
smat_ptr smat_tri(smat_ptr,const smat_ptr);//三角行列を求める
q1516_t smat_det(smat_ptr buf,const smat_ptr a);
smat_ptr smat_inv2(smat_ptr,const smat_ptr);//2x2の逆行列
//smat_ptr smat_inv(smat_ptr,const smat_ptr);//NxNの逆行列



#endif