#include "file.h"
#include "core/dynamic_array.h"
#include <stdio.h>

static u64 file_get_size(FILE* file)
{
    u64 size = 0;

    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    return size;
}

void file_read_contents(const char* filepath, dynamic_array* output, allocator* allocator)
{
    FILE* file = fopen(filepath, "r");
    if (!file)
        return;
    
    u64 size = file_get_size(file);
    dynamic_array_init(output, allocator, sizeof(u8), size);

    fread(output->data, sizeof(u8), size, file);
    output->size += size * sizeof(u8);

    fclose(file);
}