#pragma once
#include "defines.h"
#include "core/memory.h"

typedef struct arena
{
    char* buffer;
    u64 capacity;
    u64 offset;
} arena;

void arena_init(arena* arena, u64 capacityBytes);

void* arena_allocate(allocator* allocator, u64 size);
void  arena_deallocate([[maybe_unused]] allocator* allocator, [[maybe_unused]] void* address);