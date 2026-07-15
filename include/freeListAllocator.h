#ifndef FREE_LIST_ALLOCATOR_H
#define FREE_LIST_ALLOCATOR_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

static const size_t MEMORY_REGION_SIZE = 4096;

typedef struct {
    bool isFreeBlock;
    size_t padding;   
} MetaData;

typedef struct Block {
    void *ptr;
    size_t blockSize;
    struct Block *nextBlock;
    MetaData metaData; 
} Block;
 
typedef struct {
    Block headOfUsedBlocks;
    Block headOfFreeBlocks;
    void *ptrToVirtualAddressSpace;
} MemoryManager;

bool initMemoryManager(MemoryManager *memoryManager);
    

#endif
