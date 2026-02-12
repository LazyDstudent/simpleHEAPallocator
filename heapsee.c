#include "Heaps.h"

#define ALIGN16(x) (((x) + 15) & -16)

typedef struct heap_block{
    size_t size;
    int isFree;
    struct heap_block *nextHEAP;
} block_t;

static block_t *head = NULL;

void* myMALLOC(size_t size)
{
    if (size == 0)
        return NULL;

    size = ALIGN16(size);
    size_t metadata_size = ALIGN16(sizeof(block_t));
    size_t total_size = metadata_size + size;

    if (head == NULL)
    {
        block_t* block = mmap(NULL,
                              total_size,
                              PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_ANONYMOUS,
                              -1,
                              0);

        if (block == MAP_FAILED)
            return NULL;

        block->size = size;
        block->isFree = 0;
        block->nextHEAP = NULL;

        head = block;

        return (void*)((char*)block + metadata_size);
    }

    block_t* current = head;
    block_t* last = NULL;

    while (current != NULL)
    {
        if (current->isFree && current->size >= size)
        {
            if (current->size >= size + metadata_size + 16)
            {
                block_t* new_free_block = (block_t*)((char*)current + metadata_size + size);
                new_free_block->size = current->size - size - metadata_size;
                new_free_block->isFree = 1;
                new_free_block->nextHEAP = current->nextHEAP;

                current->size = size;
                current->nextHEAP = new_free_block;
            }

            current->isFree = 0;
            return (void*)((char*)current + metadata_size);
        }

        last = current;
        current = current->nextHEAP;
    }

    block_t* new_block = mmap(NULL,
                              total_size,
                              PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_ANONYMOUS,
                              -1,
                              0);

    if (new_block == MAP_FAILED)
        return NULL;

    new_block->size = size;
    new_block->isFree = 0;
    new_block->nextHEAP = NULL;

    last->nextHEAP = new_block;

    return (void*)((char*)new_block + metadata_size);
}


void myFREE(void* ptr)
{
    if (ptr == NULL)
        return;

    size_t metadata_size = ALIGN16(sizeof(block_t));
    block_t* block = (block_t*)((char*)ptr - metadata_size);

    if (block->isFree)
        return;  

    block->isFree = 1;

    if (block->nextHEAP && block->nextHEAP->isFree)
    {
        block_t* next = block->nextHEAP;

        block->size += metadata_size + next->size;
        block->nextHEAP = next->nextHEAP;
    }


    block_t* current = head;
    block_t* previous = NULL;

    while (current && current != block)
    {
        previous = current;
        current = current->nextHEAP;
    }

    if (previous && previous->isFree)
    {
        previous->size += metadata_size + block->size;
        previous->nextHEAP = block->nextHEAP;
    }
}
