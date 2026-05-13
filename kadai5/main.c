#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef uint64_t u64;
typedef int64_t  i64;
typedef uint8_t  u8;

typedef double f64;

/* Buffer */
struct
{
    u8* data;
    u64 size;
} typedef buffer;

static buffer buffer_create(u64 size)
{
    buffer result = { NULL, 0 };
    result.data = (u8*)malloc(size);
    if (result.data)
        result.size = size;
    
    return result;
}
static void buffer_destroy(buffer* buffer)
{
    if (buffer->data)
        free(buffer->data);
    buffer->size = 0;
}

/* File */
static void get_file_contents(FILE* file, buffer* buffer, int count)
{
    for (int i = 0; i < count; i++) 
        fscanf(file, "%lf", &((f64*)buffer->data)[i]);
}

/* Sorting */
static void swap(f64* lhs, f64* rhs)
{
    f64 temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

static void selection_sort(buffer buffer)
{
    f64* data = (f64*)buffer.data;
    u64 count = buffer.size / sizeof(f64);

    for (u64 i = count - 1; i > 0; --i)
    {
        u64 maxIndex = i;

        for (u64 j = 0; j < i; ++j)
        {
            if (data[j] > data[maxIndex])
                maxIndex = j;
        }

        swap(&data[i], &data[maxIndex]);
    }
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: main <input> <output>\n");
        return 1;
    }

    // Input:
    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) 
    {
        fprintf(stderr, "invalid input file\n");
        return 1;
    }

    i64 count;
    if (fscanf(inputFile, "%" SCNi64, &count) != 1 || count <= 0)
    {
        fprintf(stderr, "invalid amount of digits\n");
        return 1;
    }

    buffer inputBuffer = buffer_create(count * sizeof(f64)); 
    get_file_contents(inputFile, &inputBuffer, count);

    // Sorting:
    selection_sort(inputBuffer);

    // Output:
    FILE* outputFile = fopen(argv[2], "w");
    if (outputFile == NULL)
    {
        fprintf(stderr, "invalid output file");
        return 1;
    }

    f64* data = (f64*)inputBuffer.data;
    for(u64 i = 0; i < inputBuffer.size / sizeof(i64); ++i)
        fprintf(outputFile, "%.15g\n", data[i]);
    
    buffer_destroy(&inputBuffer);
    fclose(inputFile);
}
