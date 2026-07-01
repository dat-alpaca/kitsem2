#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>

#define NON_OWNING

#define KADAI_EXPECTED_ARGS     (4)
#define KADAI_MODE_ARG          (1)
#define KADAI_INPUT_FILE_ARG    (2)
#define KADAI_OUTPUT_FILE_ARG   (3)

typedef uint64_t u64;
typedef int64_t  i64;
typedef int32_t  i32;
typedef uint8_t  u8;
typedef float    f32;

/* core/string_utils.h */
static void strlower(char* const buffer, u64 size)
{
    for (u64 i = 0; i < size; ++i)
        buffer[i] = tolower(buffer[i]);
}

/* student/student.h */
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
    NON_OWNING struct student_node* left;
    NON_OWNING struct student_node* right;
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
            fprintf(stderr, "failed to allocate memory when reading students file.\n");
            exit(1);
        }

        newNode->data = current;
        newNode->next = NULL;
        newNode->left = NULL;
        newNode->right = NULL;

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

// student/output.h [unused]
void student_node_to_file(FILE* file, student_node* head)
{
    student_node* current = head;
    while (current != NULL) 
    {
        fprintf(file, "%g %" PRIi64 " %s\n", current->data.gpa, current->data.credits, current->data.name);
        current = current->next;
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
    char* buffer = malloc(sizeof(char) * length + 1);
    memccpy(buffer, string, sizeof(char), length);
    strlower(buffer, length);
    buffer[length] = '\0';

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
enum
{
    STUDENT_COMP_SAME = 0,
    STUDENT_COMP_NOT_SAME,
    STUDENT_COMP_LHS_GREATER = 1,
    STUDENT_COMP_RHS_GREATER = -1,
} typedef student_comparison_result;

student_comparison_result student_compare_gpa(const void* lhs, const void* rhs)
{
    float lhsGPA = ((student*)lhs)->gpa;
    float rhsGPA = ((student*)rhs)->gpa;
    
    if (lhsGPA < rhsGPA) 
        return STUDENT_COMP_RHS_GREATER;
    if (lhsGPA > rhsGPA) 
        return STUDENT_COMP_LHS_GREATER;

    return STUDENT_COMP_SAME;
}

student_comparison_result student_compare_credit(const void* lhs, const void* rhs)
{
    i32 lhsCredits = ((student*)lhs)->credits;
    i32 rhsCredits = ((student*)rhs)->credits;

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

static student_comparison_function student_comparison_function_get(student_comparison_mode mode)
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

/* student/bst_node.h [continued, depends on compare_functions.h]*/
static student_node* student_bst_insert_node(student_node* root, student_node* node, student_comparison_function comparisonFunction)
{
    if (!root)
    {
        node->left = NULL;
        node->right = NULL;
        return node;
    }

    if (comparisonFunction(&(node->data), &(root->data)) <= 0)
        root->left = student_bst_insert_node(root->left, node, comparisonFunction);
    else
        root->right = student_bst_insert_node(root->right, node, comparisonFunction);

    return root;
}


student_node* student_bst_from_linked_list(student_node* head, student_comparison_function comparisonFunction)
{
    student_node* root = NULL;
    student_node* current = head;

    while (current != NULL)
    {
        student_node* next = current->next;
        root = student_bst_insert_node(root, current, comparisonFunction);
        current = next;
    }

    return root;
}

static void student_bst_to_linked_list_inorder(student_node* root, student_node** head, student_node** tail)
{
    if (!root)
        return;

    student_bst_to_linked_list_inorder(root->left, head, tail);

    if (!*head)
    {
        *head = root;
        *tail = root;
    }
    else
    {
        (*tail)->next = root;
        *tail = root;
    }

    root->next = NULL;
    
    student_bst_to_linked_list_inorder(root->right, head, tail);
}

student_node* student_bst_to_linked_list(student_node* root)
{
    student_node* head = NULL;
    student_node* tail = NULL;
    student_bst_to_linked_list_inorder(root, &head, &tail);

    return head;
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

    // Mode:
    student_comparison_mode studentComparisonMode = student_comparison_mode_get(argv[KADAI_MODE_ARG]);
    if (studentComparisonMode == STUDENT_COMP_MODE_NONE)
    {
        fprintf(stderr, "invalid student comparison mode. available modes: gpa|credit|name>\n");
        student_node_destroy(studentHead);
        return 1;
    }
    
    // BST & Sorting:
    student_comparison_function comparisonFunction = student_comparison_function_get(studentComparisonMode);
    
    // Timing:
    {
        clock_t start = clock();

        studentHead = student_bst_from_linked_list(studentHead, comparisonFunction);
        studentHead = student_bst_to_linked_list(studentHead);

        clock_t end = clock();
        double duration = (double)(end - start) / CLOCKS_PER_SEC; 
        printf("%fs\n", duration);
    }

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