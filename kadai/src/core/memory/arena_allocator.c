#include "arena_allocator.h"
#include "core/memory.h"
#include <assert.h>
#include <stdlib.h>

void arena_init(arena* arena, u64 capacityBytes)
{
    assert(arena);

    arena->capacity = capacityBytes;
    arena->offset = 0;

    arena->buffer = malloc(capacityBytes);
    assert(arena->buffer);
}

void* arena_allocate(allocator* allocator, u64 size)
{
    arena* _arena = (arena*)allocator->context;
    u64 alignedSize = align_8(size);
    
    if (_arena->offset + alignedSize > _arena->capacity)
        return NULL;
    
    void* ptr = &_arena->buffer[_arena->offset];
    _arena->offset += alignedSize;
    
    return ptr;
}

void arena_deallocate([[maybe_unused]] allocator* allocator, [[maybe_unused]] void* address)
{
    /* Blank */
}