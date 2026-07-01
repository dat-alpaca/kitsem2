#pragma once
#include "defines.h"
#include "core/memory.h"

typedef struct bst_node
{
    void* data;
    struct bst_node* left;
    struct bst_node* right;
} bst_node;

typedef struct BST
{
    allocator* allocator;
    bst_node* root;
    u64 size;
    comparison_function comparisonFunction;
} bst;

void bst_init(bst* tree, comparison_function compare, allocator* allocator);

bool bst_insert(bst* tree, void* data);
bool bst_search(const bst* tree, const void* data);
bool bst_remove(bst* tree, const void* data);

void bst_clear(bst* tree);