#include "dynamic_array.h"
#include "core/memory.h"
#include <assert.h>
#include <string.h>

#define DYNAMIC_ARRAY_GROWTH_FACTOR (1.61803398875)

static bool dynamic_array_resize(dynamic_array* array, u64 newCapacity)
{
    assert(array);
    assert(array->allocator);

    allocator* allocator = array->allocator;

    void* newData = allocator->allocate(allocator, newCapacity * array->elementSize);
    if (!newData)
        return false;

    if (array->data && array->size > 0)
        memcpy(newData, array->data, array->size * array->elementSize);

    if (array->data)
        allocator->deallocate(allocator, array->data);

    array->data = newData;
    array->capacity = newCapacity;
    return true;
}

void dynamic_array_init(dynamic_array* array, allocator* allocator, size_t elementSize, u64 capacity)
{
    assert(array);
    assert(elementSize > 0);

    array->allocator = allocator;
    array->elementSize = elementSize;
    array->capacity = capacity;
    array->size = 0;

    if (capacity > 0)
    {
        array->data = allocator->allocate(allocator, capacity * elementSize);
        assert(array->data);
    }
    else
        array->data = NULL;
}

void* dynamic_array_get(const dynamic_array* array, size_t index)
{
    assert(array);
    assert(index < array->size);

    return (char*)array->data + (index * array->elementSize);
}

bool dynamic_array_push_back(dynamic_array* array, void* element)
{
    assert(array);
    assert(element);

    if (array->size == array->capacity)
    {
        u64 newCapacity = (u64)(array->capacity * DYNAMIC_ARRAY_GROWTH_FACTOR);
        if (newCapacity <= array->capacity)
            newCapacity = array->capacity + 8;
        
        if (!dynamic_array_resize(array, newCapacity)) 
            return false;
    }

    void* target = (char*)array->data + (array->size * array->elementSize);
    memcpy(target, element, array->elementSize);
    
    array->size++;
    return true;
}

void* dynamic_array_pop_back(dynamic_array* array)
{
    assert(array);
    assert(array->size > 0);

    --array->size;
    return (char*)array->data + (array->size * array->elementSize);
}

void dynamic_array_clear(dynamic_array* array)
{
    assert(array);
    
    if (array->data && array->allocator)
        array->allocator->deallocate(array->allocator, array->data);
    
    array->data = NULL;
    array->capacity = 0;
    array->size = 0;
}