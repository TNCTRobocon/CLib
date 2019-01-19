#ifndef __TYPES_HEADER_GUARD__
#define __TYPES_HEADER_GUARD__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef int32_t q1516_t;
typedef int64_t q3132_t;

typedef int (*comparator_t)(const void*, const void*);
typedef size_t (*hash_t)(const void*);
typedef void (*deleter_t)(void**);  //動的破壊
typedef void (*deinit_t)(void*);    //静的破壊
typedef void (*process_t)(void*);
typedef void (*process_pair_t)(void*, void*);
typedef void (*process_byte_t)(char);
typedef void** (*iterator_generator_t)(void*);
typedef void** (*iterator_stepper_t)(void**);


#endif