#include "core/dynamic_array.h"
#include "core/memory/crt_allocator.h"
#include "core/memory/arena_allocator.h"

#include "core/file.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    // CRT Allocator:
    allocator crtAllocator =
    {
        .context = NULL,
        .allocate = crt_allocate,
        .deallocate = crt_deallocate,
    };

    // Arena Allocator:
    arena arena;
    arena_init(&arena, 1024 * 1024);
    allocator arenaAllocator =
    {
        .context = &arena,
        .allocate = arena_allocate,
        .deallocate = arena_deallocate,
    };

    dynamic_array fileContents = { 0 };
    file_read_contents("input", &fileContents, &arenaAllocator);

    char* data = (char*)fileContents.data;
    for (u64 i = 0; i < fileContents.size; ++i)
    {
        printf("%c", data[i]);
    }
}