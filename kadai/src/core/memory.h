#pragma once
#include "defines.h"

typedef struct allocator
{
    void* (*allocate)(struct allocator* allocator, u64 size);
    void  (*deallocate)(struct allocator* allocator, void* address);

    void* context;
} allocator;

static inline u64 align_8(u64 value)
{
    return (value + 7) & ~7;
}