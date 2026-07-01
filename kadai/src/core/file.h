#pragma once
#include "core/memory.h"
#include "core/dynamic_array.h"

void file_read_contents(const char* filepath, dynamic_array* output, allocator* allocator);