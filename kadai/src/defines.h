#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>

#define NON_OWNING

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t   u8;
typedef uint8_t byte;

typedef int64_t  i64;
typedef int32_t  i32;

typedef float    f32;
typedef double    f64;

typedef enum : i32
{
    COMPARE_SAME = 0,
    COMPARE_LHS_GREATER = 1,
    COMPARE_RHS_GREATER = -1
} compare_result;

typedef compare_result (*comparison_function)(const void* lhs, const void* rhs);
