#pragma once
#include "core/memory.h"

void* crt_allocate([[maybe_unused]] allocator* allocator, u64 size);
void crt_deallocate([[maybe_unused]] allocator* allocator, void* address);