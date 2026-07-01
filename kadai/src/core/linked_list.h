#pragma once
#include "core/memory.h"

struct linked_list_node
{
    void* data;
    struct linked_list_node* next; 
} typedef linked_list_node;

typedef struct linked_list
{
    allocator* allocator;
    linked_list_node* head;
    linked_list_node* tail;
    u64 size;
} linked_list;

void linked_list_init(linked_list* list, allocator* allocator);

bool linked_list_push_back(linked_list* list, void* data);
void* linked_list_pop_back(linked_list* list);

void linked_list_clear(linked_list* list);