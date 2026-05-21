#include <stdio.h>
struct
{
    void* ptr;
    int data;    
} typedef node;


int main()
{   
    node* node = 0;

    // same as:
    if (node->ptr || node == 0) // this comes before; node->ptr is undefined since node is 0
    {
        printf("pointer null");

    }
    if (node) // this comes after
    {

    }

    return 1;
}