#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "freeListAllocator.h"

bool initFreeList(FreeList *freeList) {
    freeList -> ptrToVirtualAddressSpace = (void*) mmap(NULL, MEMORY_REGION_SIZE,
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
            -1, 0);
    
    if (freeList -> ptrToVirtualAddressSpace == MAP_FAILED ||
            freeList -> ptrToVirtualAddressSpace == NULL) {
        fprintf(stderr, "Error: Failed to initialise free list as mmap failed.\n");
        return false;
    }

    freeList -> head.blockSize = MEMORY_REGION_SIZE;
    freeList -> head.ptr = freeList -> ptrToVirtualAddressSpace;
    freeList -> head.nextFreeBlock = NULL;
    
    return true;
}

static bool validateFreeListInit(FreeList *freeList) {
    if (freeList == NULL) {
        fprintf(stderr, "Error: Failed to validate the initialisation of freeList as freeList == NULL.\n");
        return false;
    } 

    if (freeList -> ptrToVirtualAddressSpace == MAP_FAILED) {
        fprintf(stderr, "Error: Failed to validate the initialisation of freeList as ptrToVirtualAddressSpace == MAP_FAILED.\n");
        return false;
    }

    if (freeList -> ptrToVirtualAddressSpace == NULL) {
        fprintf(stderr, "Error: Failed to validate the initialisation of freeList as ptrToVirtualAddressSpace == NULL.\n");
        return false;
    }

    if (freeList -> head.blockSize != MEMORY_REGION_SIZE ||
            freeList -> head.ptr != freeList -> ptrToVirtualAddressSpace ||
            freeList -> head.nextFreeBlock != NULL) {
        fprintf(stderr, "Error: Failed to validate the initialisation of freeList as freeList -> head was not initialised correctly.\n");
        return false;
    }

    return true;
}

static bool validateFreeList(FreeList *freeList) {
    if (freeList == NULL) {
        fprintf(stderr, "Error: Failed to validate the initialisation of freeList as freeList == NULL.\n");
        return false;
    } 

    if (freeList -> ptrToVirtualAddressSpace == MAP_FAILED) {
        fprintf(stderr, "Error: Failed to validate the initialisation of freeList as ptrToVirtualAddressSpace == MAP_FAILED.\n");
        return false;
    }

    if (freeList -> ptrToVirtualAddressSpace == NULL) {
        fprintf(stderr, "Error: Failed to validate the initialisation of freeList as ptrToVirtualAddressSpace == NULL.\n");
        return false;
    }

    return true;
}





















