#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "freeListAllocator.h" 

static MetaData makeMetaData(bool isFreeBlock, size_t padding) {
    MetaData data;
    
    data.isFreeBlock = isFreeBlock;
    data.padding = padding;
    
    return data;
}

static bool initHeadOfFreeBlocks(MemoryManager *memoryManager, Block *headOfFreeBlocks) {
    if (headOfFreeBlocks == (Block*) NULL || headOfFreeBlocks == NULL) {
        fprintf(stderr, "Error: Failed to init head of free blocks as headOfFreeBlocks == NULL.\n");
        return false;  
    }

    headOfFreeBlocks -> ptr = memoryManager -> ptrToVirtualAddressSpace; 
    headOfFreeBlocks -> blockSize = MEMORY_REGION_SIZE;
    headOfFreeBlocks -> nextBlock = (Block*) NULL;
    headOfFreeBlocks -> metaData = makeMetaData(true, 0);

    return true;
}

static bool initHeadOfUsedBlocks(MemoryManager *memoryManager, Block *headOfUsedBlocks) {
    if (headOfUsedBlocks == (Block*) NULL || headOfUsedBlocks == NULL) {
        fprintf(stderr, "Error: Failed to init head of used blocks as headOfUsedBlocks == NULL.\n");
        return false;  
    }

    headOfUsedBlocks -> ptr = (void*) NULL;
    headOfUsedBlocks -> blockSize = 0;
    headOfUsedBlocks -> nextBlock = (Block*) NULL;
    headOfUsedBlocks -> metaData = makeMetaData(false, 0);
    
    return true;
}

bool initMemoryManager(MemoryManager *memoryManager) {
    if (memoryManager == NULL || memoryManager == (MemoryManager*) NULL) {
        fprintf(stderr, "Error: Failed to init memoryManager as invalid manager was passed in: memoryManager == NULL.\n");
        return false;
    }
    
    memoryManager -> ptrToVirtualAddressSpace = (void*) mmap(NULL, MEMORY_REGION_SIZE,
            PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS,
            -1, 0);
    
    if (memoryManager -> ptrToVirtualAddressSpace == MAP_FAILED) {
        fprintf(stderr, "Error: Failed to init memory manager as mapping failed.\n");
        return false;
    }

    if (!initHeadOfFreeBlocks(memoryManager, &memoryManager -> headOfFreeBlocks)) {
        fprintf(stderr, "Error: Failed to init memoryManager as init of headOfFreeBlocks failed.\n");
        return false;
    }

    if (!initHeadOfUsedBlocks(memoryManager, &memoryManager -> headOfUsedBlocks)) {
        fprintf(stderr, "Error: Failed to init memoryManager as init of headOfUsedBlocks failed.\n");
        return false;
    }

    return true;
}

















