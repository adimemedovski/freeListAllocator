#ifndef FREE_LIST_ALLOCATOR_H
#define FREE_LIST_ALLOCATOR_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_MEMORY_SIZE 4096

/*
 * When freeing memory, the total block size will be:
 *     allocatedMemory + sizeof(metaData) + totalPadding
 *
 * How do we determine what totalPadding is? Well, we need to take 
 * into account three things:
 *     1. The alignmentPadding of the pointer;
 *     2. the alignmentPadding of the meta data;
 *     3. the alignmentPadding of the block header.
 */

typedef struct {
    size_t padding;
    size_t allocatedMemory;
} MetaData;

typedef struct Block {
    struct Block *nextBlock;
    size_t blockSize;
} Block;

typedef struct {
    void *memoryBasePtr; 
    Block *head; 
} FreeList;


#endif
