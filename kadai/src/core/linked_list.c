#include "linked_list.h"
#include <assert.h>

void linked_list_init(linked_list* list, allocator* allocator)
{
    assert(list);
    assert(allocator);

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->allocator = allocator;
}

bool linked_list_push_back(linked_list* list, void* data)
{
    assert(data);
    assert(list);
    assert(list->allocator);

    allocator* allocator = list->allocator;
    
    linked_list_node* node = (linked_list_node*)allocator->allocate(allocator, sizeof(linked_list_node));
    if (!node)
        return false;

    if (list->tail == NULL)
    {
        list->head = node;
        list->tail = node;
    } 
    else 
    {
        list->tail->next = node;
        list->tail = node;
    }

    ++list->size;
    return true;
}

void* linked_list_pop_back(linked_list* list)
{
    assert(list);
    assert(list->allocator);

    if (!list->head)
        return NULL;

    linked_list_node* previousTail = list->tail;
    void* data = previousTail->data;

    if (list->head == list->tail)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        linked_list_node* current = list->head;
        while (current->next != list->tail)
            current = current->next;
        
        current->next = NULL;
        list->tail = current;
    }

    list->allocator->deallocate(list->allocator, previousTail);
    --list->size;

    return data;
}

void linked_list_clear(linked_list* list)
{
    while (list->tail != NULL)
        linked_list_pop_back(list);
}