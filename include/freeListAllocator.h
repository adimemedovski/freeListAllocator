#ifndef FREE_LIST_ALLOCATOR_H
#define FREE_LIST_ALLOCATOR_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

static const size_t MEMORY_REGION_SIZE = 4096;

typedef struct {
    size_t allocatedMemory;
    size_t padding; 
} MetaData;

typedef struct FreeBlock {
    void *ptr;
    size_t blockSize;
    struct FreeBlock *nextFreeBlock;
} FreeBlock;

typedef struct {
    void *ptrToVirtualAddressSpace;
    FreeBlock head;
} FreeList;

bool initFreeList(FreeList *freeList);

#endif
