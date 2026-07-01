#include "bst.h"
#include "defines.h"
#include <assert.h>

// Helper functions:
static bst_node* bst_allocate_node(allocator* allocator, void* data)
{
    bst_node* node = (bst_node*)allocator->allocate(allocator, sizeof(bst_node));
    if (node)
    {
        node->data = data;
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

static bst_node* bst_insert_node(bst_node* root, bst_node* node, comparison_function comparisonFunction, bool* success)
{
    if (!root)
    {
        *success = true;
        return node;
    }

    compare_result cmp = comparisonFunction(root->data, node->data);
    
    if (cmp == COMPARE_LHS_GREATER)
        root->left = bst_insert_node(root->left, node, comparisonFunction, success);
    
    else if (cmp == COMPARE_RHS_GREATER)
        root->right = bst_insert_node(root->right, node, comparisonFunction, success);
    
    else if (cmp == COMPARE_SAME)
        *success = false;
    
    return root;
}

static void bst_clear_recursive(bst_node* root, allocator* allocator)
{
    if (!root)
        return;

    bst_clear_recursive(root->left, allocator);
    bst_clear_recursive(root->right, allocator);

    allocator->deallocate(allocator, root);
}

static bst_node* bst_find_minimum(bst_node* node)
{
    while (node && node->left)
        node = node->left;

    return node;
}

static bst_node* bst_remove_recursive(bst_node* root, const void* data, comparison_function comparisonFunction, allocator* allocator, bool* success)
{
    if (!root)
        return NULL;

    compare_result cmp = comparisonFunction(data, root->data);
    
    if (cmp < 0)
        root->left = bst_remove_recursive(root->left, data, comparisonFunction, allocator, success);
    else if (cmp > 0)
        root->right = bst_remove_recursive(root->right, data, comparisonFunction, allocator, success);
    else
    {
        *success = true;

        // One child:
        if (!root->left)
        {
            bst_node* nextRight = root->right;
            allocator->deallocate(allocator, root);
            return nextRight;
        }
        else if (!root->right)
        {
            bst_node* nextLeft = root->left;
            allocator->deallocate(allocator, root);
            return nextLeft;
        }

        // Two children:
        bst_node* minimumNode = bst_find_minimum(root->right);
        root->data = minimumNode->data;

        root->right = bst_remove_recursive(root->right, minimumNode->data, comparisonFunction, allocator, success);
    }

    return root;
}

// BST:
void bst_init(bst* tree, comparison_function comparisonFunction, allocator* allocator)
{
    assert(tree);

    tree->root = NULL;
    tree->size = 0;
    tree->allocator = allocator;
    tree->comparisonFunction = comparisonFunction;
}

bool bst_insert(bst* tree, void* data)
{
    assert(tree);
    assert(data);
    assert(tree->allocator);

    bst_node* node = bst_allocate_node(tree->allocator, data);
    if (!node)
        return false;

    bool success = false;
    tree->root = bst_insert_node(tree->root, node, tree->comparisonFunction, &success);
    
    if (success)
        ++tree->size;
    else
        tree->allocator->deallocate(tree->allocator, node);

    return success;
}

bool bst_search(const bst* tree, const void* data)
{
    assert(tree);
    assert(data);

    bst_node* current = tree->root;
    while (current)
    {
        compare_result cmp = tree->comparisonFunction(data, current->data);

        if (cmp == 0)
            return true;

        current = (cmp < 0) ? current->left : current->right;
    }
    return false;
}

bool bst_remove(bst* tree, const void* data)
{
    assert(data);
    assert(tree);
    assert(tree->allocator);

    bool success = false;
    tree->root = bst_remove_recursive(tree->root, data, tree->comparisonFunction, tree->allocator, &success);

    if (success)
        tree->size--;
    
    return success;
}

void bst_clear(bst* tree)
{
    assert(tree);
    assert(tree->allocator);

    bst_clear_recursive(tree->root, tree->allocator);
    tree->root = NULL;
    tree->size = 0;
}