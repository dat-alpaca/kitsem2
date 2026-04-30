#include <stdio.h>
#include <stdint.h>

#define MAX_N 5

typedef uint32_t u32;
typedef float    f32;
typedef const f32 (*const_matrix)[MAX_N];
typedef f32 (*matrix)[MAX_N];

/* Utils */
void swap(matrix* A, matrix* B)
{
    matrix temp = *A;
    *A = *B;
    *B = temp;
}

/* Matrices */
void matrix_zero(u32 n, matrix mat)
{
    for(u32 i = 0; i < n; ++i)
    {
        for(u32 j = 0; j < n; ++j)
            mat[i][j] = 0.0f;
    }
}

void matrix_identity(u32 n, matrix mat)
{
    matrix_zero(n, mat);

    for (u32 i = 0; i < n; ++i)
        mat[i][i] = 1.0f;
}

void matrix_get_user_input(u32 n, matrix mat)
{
    for (u32 i = 0; i < n; ++i)
    {
        printf("Column #%d: ", i);
        for (u32 j = 0; j < n; j++)
            scanf("%f", &mat[i][j]);
    }
}

void matrix_display(u32 n, const_matrix mat)
{
    for (u32 i = 0; i < n; ++i) 
    {
        for (u32 j = 0; j < n; j++) 
            printf("%.2f ", mat[i][j]);
        printf("\n");
    }
}

void matrix_multiply(u32 n, const_matrix X, const_matrix Y, matrix Z) 
{
    matrix_zero(n, Z);

    for (u32 k = 0; k < n; ++k)
    {
        for (u32 i = 0; i < n; ++i) 
        {
            const f32 r = X[i][k];
        
            for (u32 j = 0; j < n; ++j)
                Z[i][j] += r * Y[k][j];
        }
    }
}

matrix matrix_power(u32 n, u32 k, matrix A, matrix temp0, matrix temp1)
{
    matrix result = temp0;
    matrix acc = temp1;
    
    matrix_identity(n, result);

    for (u32 i = 0; i < k; ++i) 
    {
        matrix_multiply(n, A, result, acc);
        swap(&result, &acc);
    }
    return result;
}

int main()
{
    u32 n, k;

    float  A[MAX_N][MAX_N];
    float M1[MAX_N][MAX_N];
    float M2[MAX_N][MAX_N];

    printf("Value of n, k: ");
    if (scanf("%u %u", &n, &k) != 2 || n < 1 || n > MAX_N) 
    {
        fprintf(stderr, "Invalid input\n");    
        return 1;
    }

    matrix_zero(n, A);
    matrix_get_user_input(n, A);

    matrix result = matrix_power(n, k, A, M1, M2);
    matrix_display(n, result);

    return 0;
}