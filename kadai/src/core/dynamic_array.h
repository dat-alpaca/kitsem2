#pragma once
#include "core/memory.h"

typedef struct dynamic_array
{
    allocator* allocator;
    void* data;
    size_t capacity;
    size_t size;
    size_t elementSize;
} dynamic_array;

void dynamic_array_init(dynamic_array* array, allocator* allocator, size_t elementSize, u64 capacity);
void* dynamic_array_get(const dynamic_array* array, size_t index);

bool dynamic_array_push_back(dynamic_array* array, void* element);
void* dynamic_array_pop_back(dynamic_array* array);

void dynamic_array_clear(dynamic_array* array);