#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "freeListAllocator.h"

bool initFreeList(FreeList *freeList) {
    freeList -> basePtr = (void*) mmap(NULL, MEMORY_REGION_SIZE,
            PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS,
            -1, 0); 
    
    if (freeList -> basePtr == MAP_FAILED || freeList -> basePtr == NULL) {
        fprintf(stderr, "Error: Failed to initialise free list as basePtr failed mapping.\n");
        return false;
    }

    freeList -> nextFreeBlock.ptr = freeList -> basePtr;
    freeList -> nextFreeBlock.blockSize = MEMORY_REGION_SIZE;
    freeList -> nextFreeBlock.nextBlock = (Block*) NULL; 
    
    return true;
}

static bool validateFreeListInit(FreeList *freeList) {
    if (freeList == (FreeList*) NULL || freeList == NULL) {
        fprintf(stderr, "Error: Failed to validate free list's initialisation as freeList is NULL.\n");
        return false;
    }

    if (freeList -> basePtr == MAP_FAILED || freeList -> basePtr == NULL) {
        fprintf(stderr, "Error: Failed to validate free list's initialisation as base ptr was not initialised properly.\n");
        return false;
    }
    
    if (freeList -> nextFreeBlock.blockSize != MEMORY_REGION_SIZE) {
        fprintf(stderr, "Error: Failed to validate free list's initialisation as the block size of nextFreeBlock is not MEMORY_REGION_SIZE.\n");
        return false;
    }

    if (freeList -> nextFreeBlock.ptr != freeList -> basePtr) {
        fprintf(stderr, "Error: Failed to validate free list's initialisation as nextFreeBlock.ptr != freeList -> basePtr.\n");
        return false;
    }

    if (freeList -> nextFreeBlock.nextBlock != (Block*) NULL) {
        fprintf(stderr, "Error: Failed to validate free list's initialsiation as nextfreeBlock.nextBlock != (Block*) NULL");
        return false;
    }

    return true;
}

static bool validateFreeList(FreeList *freeList) {
    if (freeList == (FreeList*) NULL || freeList == NULL) {
        fprintf(stderr, "Failed to validate free list as freeList == NULL.\n");
        return false;
    }
    
    if (freeList -> basePtr == MAP_FAILED || freeList -> basePtr == NULL) {
        fprintf(stderr, "Error: Failed to validate free list as basePtr failed mapping.\n");
        return false;
    }

    return true;
}

/*
 * Careful when using this function; ensure to not pass a blockSize of 0
 * to this function. Reinforce that ptr and nextBlock are valid pointers.
 */
static Block makeBlock(size_t blockSize, void *ptr, Block *nextBlock) {
    Block block;

    block.blockSize = blockSize;
    block.ptr = ptr;
    block.nextBlock = nextBlock;
    
    return block;
}








