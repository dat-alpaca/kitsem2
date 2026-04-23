#include <stdio.h>
#include <stdint.h>

#define MAX_N 5

typedef uint32_t u32;
typedef float    f32;
typedef const f32 (*const_matrix)[MAX_N];
typedef f32 (*matrix)[MAX_N];

void matrix_zero(u32 n, matrix mat)
{
    for(u32 i = 0; i < n; ++i)
    {
        for(u32 j = 0; j < n; ++j)
            mat[i][j] = 0.0f;
    }
}

void get_user_matrix_input(u32 n, matrix mat)
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

void matrix_multiply(u32 n, const_matrix restrict X, const_matrix restrict Y, matrix restrict Z) 
{
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

int main()
{
    u32 n;
    float A1[MAX_N][MAX_N];
    float A2[MAX_N][MAX_N];

    printf("Value of n: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N || n <= 0)
    {
        fprintf(stderr, "Invalid input\n");    
        return 1;
    }

    matrix_zero(n, A1);
    matrix_zero(n, A2);

    get_user_matrix_input(n, A1);
    matrix_multiply(n, A1, A1, A2);
    matrix_display(n, A2);

    return 0;
}