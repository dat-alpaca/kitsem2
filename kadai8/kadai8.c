#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#define KADAI_EXPECTED_ARGS     (4)
#define KADAI_MODE_ARG          (1)
#define KADAI_INPUT_FILE_ARG    (2)
#define KADAI_OUTPUT_FILE_ARG   (3)

typedef uint64_t u64;
typedef int64_t  i64;
typedef int32_t  i32;
typedef uint8_t  u8;
typedef float    f32;

/* student/student.h */
// 0.25 KiB; 8 byte aligned
typedef struct
{
    i64 credits;
    f32 gpa;
    char name[244];
} student;

/* student/student_node.h */
typedef struct student_node
{
    student data;
    struct student_node* next;
} student_node;

void student_node_destroy(student_node* head)
{
    student_node* current = head;
    while (current != NULL)
    {
        student_node* next = current->next;
        free(current);
        current = next;
    }
}

// student/input.h
student_node* student_node_from_file(FILE* file)
{
    student_node* head = NULL, *tail = NULL;
    while (true)
    {
        student current;
        if (fscanf(file, "%f %" SCNi64, &current.gpa, &current.credits) != 2)
            break;

        fgetc(file);
        
        if (fscanf(file, " %[^\n]", current.name) != 1)
            break;

        student_node* newNode = (student_node*)malloc(sizeof(student_node));
        if (newNode == NULL)
        {
            fprintf(stderr, "Failed to allocate memory when reading student file.\n");
            exit(1);
        }

        newNode->data = current;
        newNode->next = NULL;

        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        } 
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    return head;
}

// student/output.h
void student_node_to_file(FILE* file, student_node* head)
{
    student_node* current = head;
    while (current != NULL) 
    {
        fprintf(file, "%g %" PRIi64 " %s\n", current->data.gpa, current->data.credits, current->data.name);
        current = current->next;
    }
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

    student_node* studentHead = student_node_from_file(inputFile);
    fclose(inputFile);

    // Output:
    FILE* outputFile = fopen(argv[KADAI_OUTPUT_FILE_ARG], "w");
    if (outputFile == NULL)
    {
        fprintf(stderr, "invalid output file\n");
        student_node_destroy(studentHead);
        return 1;
    }

    student_node_to_file(outputFile, studentHead);
    fclose(outputFile);

    student_node_destroy(studentHead);

    return 0;
}