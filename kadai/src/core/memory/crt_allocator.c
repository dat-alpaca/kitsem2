#include "crt_allocator.h"
#include <assert.h>
#include <stdlib.h>

void* crt_allocate([[maybe_unused]] allocator* allocator, u64 size)
{
    return malloc(size);
}

void crt_deallocate([[maybe_unused]] allocator* allocator, void* address)
{
    assert(address);
    free(address);
}