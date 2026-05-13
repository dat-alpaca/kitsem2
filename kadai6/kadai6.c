#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

/* core/defines.h */
#define KADAI_MAX_BUFFER_COUNT (1 << 20)

#define KADAI_EXPECTED_ARGS (4)
#define KADAI_MODE_ARG (1)
#define KADAI_INPUT_FILE_ARG (2)
#define KADAI_OUTPUT_FILE_ARG (3)

typedef uint64_t u64;
typedef int64_t  i64;
typedef int32_t  i32;
typedef uint8_t   u8;

typedef float    f32;

/* core/buffer.h */
struct
{
    void* data;
    u64 maxCount;
    u64 elementSize;
    u64 current;
    u64 capacity;
} typedef buffer;

void buffer_init(buffer* const buffer, u64 maxCount, u64 elementSize)
{
    assert(buffer);
    assert(maxCount > 0 && maxCount <= KADAI_MAX_BUFFER_COUNT);

    buffer->maxCount = maxCount;
    buffer->elementSize = elementSize;
    buffer->current = 0;
    buffer->capacity = maxCount * elementSize;

    buffer->data = malloc(buffer->capacity);
    assert(buffer->data);
}

void* buffer_get(buffer* const buffer, u64 index)
{
    assert(index >= 0);
    assert(buffer);
    assert(buffer->data);
    assert(buffer->elementSize * index <= buffer->current);
    return &buffer[index * buffer->elementSize];
}

u64 buffer_size(buffer* const buffer)
{
    return buffer->current / buffer->elementSize; 
}

void buffer_push(buffer* const buffer, void* data, u64 count)
{
    assert(data);
    assert(buffer);
    assert(count > 0);
    assert(buffer->data);

    u64 byteCount = buffer->elementSize * count;
    assert(buffer->current + byteCount <= buffer->capacity);

    u8* arenaData = (u8*)buffer->data;
    u8* byteData = (u8*)data;
    for (u64 i = 0; i < byteCount; ++i)
        arenaData[buffer->current + i] = byteData[i];

    buffer->current += byteCount;
}

void buffer_pop(buffer* const buffer, u64 count)
{
    u64 byteCount = buffer->elementSize * count;

    assert(buffer);
    assert(count > 0);
    assert(buffer->data);
    assert(buffer->current >= byteCount);

    buffer->current -= byteCount;
}

void buffer_destroy(buffer* buffer)
{
    assert(buffer);
    assert(buffer->data);
    assert(buffer->capacity);
    
    free(buffer->data);
    buffer = NULL;
}

/* core/string_utils.h */
static void strlower(char* const buffer, u64 size)
{
    for (u64 i = 0; i < size; ++i)
        buffer[i] = tolower(buffer[i]);
}

/* student/student.h */
// 0.25 KiB; 8 byte aligned
struct
{
    i64 credits;
    f32 gpa;
    char name[244];
} typedef student;

static void student_from_file(FILE* file, buffer* const students, u64 count)
{
    for (u64 i = 0; i < count; i++)
    {
        student current; 
        fscanf(file, "%f %ld", &current.gpa, &current.credits);
        fgetc(file);
        fscanf(file, " %[^\n]", current.name);

        buffer_push(students, &current, 1);
    }
}

static void student_array_to_file(FILE* file, buffer* students)
{
    student* studentData = (student*)students->data;
    for (u64 i = 0; i < buffer_size(students); ++i)
    {
        student* current = &studentData[i];
        fprintf(file, "%g %ld %s\n", current->gpa, current->credits, current->name);
    }
}

/* student/compare_mode.h */
enum
{
    STUDENT_COMP_MODE_NONE = 0,
    STUDENT_COMP_MODE_GPA,
    STUDENT_COMP_MODE_CREDITS,
    STUDENT_COMP_MODE_NAME,
} typedef student_comparison_mode;

struct
{
    const char* name;
    student_comparison_mode mode;
} typedef student_comparison_mode_table_item;

static student_comparison_mode_table_item gStudentComparisonModeTable[] =
{
    { "g", STUDENT_COMP_MODE_GPA },
    { "gpa", STUDENT_COMP_MODE_GPA },
    { "gpas", STUDENT_COMP_MODE_GPA },
    
    { "c", STUDENT_COMP_MODE_CREDITS },
    { "credit", STUDENT_COMP_MODE_CREDITS },
    { "credits", STUDENT_COMP_MODE_CREDITS },
    
    { "n", STUDENT_COMP_MODE_NAME },
    { "name", STUDENT_COMP_MODE_NAME },
    { "names", STUDENT_COMP_MODE_NAME },
};

student_comparison_mode student_comparison_mode_get(const char* string)
{
    u64 length = strlen(string);
    char* buffer = malloc(sizeof(char) * length);
    memccpy(buffer, string, sizeof(char), length);
    strlower(buffer, length);

    size_t itemCount = sizeof(gStudentComparisonModeTable) / sizeof(student_comparison_mode_table_item);
    student_comparison_mode mode = STUDENT_COMP_MODE_NONE;
    for (u64 i = 0; i < itemCount; ++i)
    {
        student_comparison_mode_table_item* current = &gStudentComparisonModeTable[i];
        if (strcmp(current->name, buffer) == 0)
        {
            mode = current->mode;
            break;
        }
    }

    free(buffer);
    return mode;
}

/* student/compare_functions.h */
enum : i32
{
    STUDENT_COMP_SAME = 0,
    STUDENT_COMP_NOT_SAME,
    STUDENT_COMP_LHS_GREATER = 1,
    STUDENT_COMP_RHS_GREATER = -1,
} typedef student_comparison_result;

student_comparison_result student_compare_gpa(const void* lhs, const void* rhs)
{
    float lhsGPA = ((student *)lhs)->gpa;
    float rhsGPA = ((student *)rhs)->gpa;
    
    if (lhsGPA < rhsGPA) 
        return STUDENT_COMP_RHS_GREATER;
    if (lhsGPA > rhsGPA) 
        return STUDENT_COMP_LHS_GREATER;

    return STUDENT_COMP_SAME;
}

student_comparison_result student_compare_credit(const void* lhs, const void* rhs)
{
    i32 lhsCredits = ((student *)lhs)->credits;
    i32 rhsCredits = ((student *)rhs)->credits;

    if (lhsCredits < rhsCredits) 
        return STUDENT_COMP_RHS_GREATER;
    if (lhsCredits > rhsCredits) 
        return STUDENT_COMP_LHS_GREATER;

    return STUDENT_COMP_SAME;
}

student_comparison_result student_compare_name(const void* lhs, const void* rhs)
{
    student* lhsStudent = (student*)lhs;
    student* rhsStudent = (student*)rhs;

    i32 result = strcmp(lhsStudent->name, rhsStudent->name);

    if (result < 0) 
        return STUDENT_COMP_RHS_GREATER;
    if (result > 0) 
        return STUDENT_COMP_LHS_GREATER;

    return STUDENT_COMP_SAME;
}

typedef student_comparison_result (*student_comparison_function)(const void* lhs, const void* rhs);

struct
{
    student_comparison_mode mode;
    student_comparison_function function;
} typedef student_comparison_function_table_item;

static student_comparison_function_table_item gStudentComparisonFunctionTable[] =
{
    { STUDENT_COMP_MODE_GPA, student_compare_gpa },
    { STUDENT_COMP_MODE_CREDITS, student_compare_credit },
    { STUDENT_COMP_MODE_NAME, student_compare_name },
};

student_comparison_function student_comparison_function_get(student_comparison_mode mode)
{
    size_t itemCount = sizeof(gStudentComparisonFunctionTable) / sizeof(student_comparison_function_table_item);
    student_comparison_function function = NULL;
    for (u64 i = 0; i < itemCount; ++i)
    {
        student_comparison_function_table_item* current = &gStudentComparisonFunctionTable[i];
        if (current->mode != mode)
            continue;

        function = current->function;
        break;
    }

    return function;
}

int main(int argc, char** argv)
{
    if (argc != KADAI_EXPECTED_ARGS)
    {
        fprintf(stderr, "usage: %s <mode: gpa|credit|name> <input> <output>\n", argv[0]);
        return 1;
    }

    // Input:
    FILE* inputFile = fopen(argv[KADAI_INPUT_FILE_ARG], "r");
    if (inputFile == NULL) 
    {
        fprintf(stderr, "invalid input file\n");
        return 1;
    }

    i64 count;
    if (fscanf(inputFile, "%" SCNi64, &count) != 1 || count <= 0)
    {
        fprintf(stderr, "invalid input file count\n");
        return 1;
    }

    buffer inputBuffer;
    buffer_init(&inputBuffer, count, sizeof(student));
    student_from_file(inputFile, &inputBuffer, count);

    // Sorting:
    student_comparison_mode studentComparisonMode = student_comparison_mode_get(argv[KADAI_MODE_ARG]);
    if (studentComparisonMode == STUDENT_COMP_MODE_NONE)
    {
        fprintf(stderr, "invalid student comparison mode. available modes: gpa|credit|name>\n");
        return 1;
    }

    student_comparison_function comparisonFunction = student_comparison_function_get(studentComparisonMode);
    qsort(inputBuffer.data, count, sizeof(student), comparisonFunction);

    // Output:
    FILE* outputFile = fopen(argv[KADAI_OUTPUT_FILE_ARG], "w");
    if (outputFile == NULL)
    {
        fprintf(stderr, "invalid output file");
        return 1;
    }

    student_array_to_file(outputFile, &inputBuffer);
    
    buffer_destroy(&inputBuffer);
    fclose(inputFile);
}
