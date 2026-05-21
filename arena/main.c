#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>

#include <stdio.h>

#define KADAI_ARENA_PAGE_MAX_COUNT 256

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t   u8;

/* stack-based arena allocator */
struct
{
    void* data;
    u64 pageCount;
    u64 elementSize;
    u64 current;
    u64 capacity;
} typedef buffer;

void uniform_stack_arena_init(buffer* const arena, u64 pageCount, u64 elementSize)
{
    assert(arena);
    assert(pageCount > 0 && pageCount <= KADAI_ARENA_PAGE_MAX_COUNT);

    arena->pageCount = pageCount;
    arena->elementSize = elementSize;
    arena->current = 0;
    arena->capacity = sysconf(_SC_PAGESIZE) * pageCount;

    arena->data = mmap(NULL, arena->capacity, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(arena->data);
}

void* buffer_get(buffer* const arena, u64 index)
{
    assert(index >= 0);
    assert(arena);
    assert(arena->data);
    assert(arena->elementSize * index <= arena->current);
    return &arena[index * arena->elementSize];
}

void uniform_stack_arena_push(buffer* const arena, void* data, u64 count)
{
    assert(data);
    assert(arena);
    assert(count > 0);
    assert(arena->data);

    u64 byteCount = arena->elementSize * count;
    assert(arena->current + byteCount <= arena->capacity);

    u8* arenaData = (u8*)arena->data;
    u8* byteData = (u8*)data;
    for (u64 i = 0; i < byteCount; ++i)
        arenaData[arena->current + i] = byteData[i];

    arena->current += byteCount;
}

void buffer_pop(buffer* const arena, u64 count)
{
    u64 byteCount = arena->elementSize * count;

    assert(arena);
    assert(count > 0);
    assert(arena->data);
    assert(arena->current >= byteCount);

    arena->current -= byteCount;
}

void buffer_destroy(buffer* arena)
{
    assert(arena);
    assert(arena->data);
    assert(arena->capacity);
    
    munmap(arena->data, arena->capacity);
    arena = NULL;
}

int main()
{
    buffer arena = { 0 };
    uniform_stack_arena_init(&arena, 1, sizeof(int));

    int data = 32;
    uniform_stack_arena_push(&arena, &data, 1);

    buffer_destroy(&arena);
}