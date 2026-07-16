#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "freeListAllocator.h" 


bool initFreeList(FreeList *freeList) {
    if (freeList == (FreeList*) NULL) {
        fprintf(stderr, "Error: Failed to init freeList as freeList is NULL.\n");
        return false;
    }

    freeList -> memoryBasePtr = (void*) mmap(NULL, MAX_MEMORY_SIZE,
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
            -1, 0);
 
    if (freeList -> memoryBasePtr == NULL) {
        fprintf(stderr, "Error: Failed to init freeList as basePtr is NULL.\n");
        return false;
    }
   
    if (freeList -> memoryBasePtr == MAP_FAILED) {
        fprintf(stderr, "Error: Failed to init freeList as mmap returned MAP_FAILED.\n");
        return false;
    }
    
    freeList -> head = (Block*) freeList -> memoryBasePtr;
    freeList -> head -> blockSize = MAX_MEMORY_SIZE;
    freeList -> head -> nextBlock = (Block*) NULL;
    
    return true;
}

static size_t getAlignmentPadding(void* ptr, size_t alignment) {
    if (ptr == NULL) {
        fprintf(stderr, "Error: Failed to get alignment padding as ptr == NULL.\n");
        return false;
    }

    if (alignment == 0) {
        fprintf(stderr, "Error: Failed to get alignment padding as alignment cannot be 0 -- division by 0 error.\n");
        return false;
    }

    size_t address = (size_t) ptr;
    
    if (address % alignment == 0) {
        return 0;
    }

    return alignment - (address % alignment);
}


void *freeListAlloc(FreeList *freeList, size_t blockSize, size_t alignment) {
    void *ptrMetaData = (void*) freeList -> head;
    MetaData *metaData = (MetaData*) ptrMetaData;
    metaData -> alignmentPaddingOfMetaData = getAlignmentPadding(ptrMetaData, _Alignof(MetaData));
    metaData += metaData -> alignmentPaddingOfMetaData;
   
    char* ptrAllocatedPtr = (char*) ptrMetaData;
    ptrAllocatedPtr += metaData -> alignmentPaddingOfMetaData + sizeof(MetaData);

    char* newPtr = ptrAllocatedPtr + blockSize;
      
    if (freeList -> head -> nextBlock == NULL || freeList -> head -> nextBlock == (Block* ) NULL) {
        size_t* newPosOfHead = (size_t*) freeList -> head; 
        newPosOfHead += sizeof(MetaData) + metaData -> alignmentPaddingOfMetaData; 
    } else {
        freeList -> head = freeList -> head -> nextBlock; 
    } 

    return (void*) ptrAllocatedPtr;
}

bool freeAlloc(FreeList *freeList, void *ptr) {
    char *freePtr = (char*) ptr;
    
    // We will assume that metaData is contigious to the ptr. 
    freePtr -= sizeof(MetaData);
    MetaData *metaData = (MetaData*) freePtr;
    
    Block *freedBlock = (Block*) metaData; 
    freedBlock -> blockSize = metaData -> allocatedMemory + metaData -> alignmentPaddingOfPtr + metaData -> alignmentPaddingOfMetaData;
    
    freedBlock -> nextBlock = freeList -> head;
    freeList -> head = freedBlock;
    
    return true;
}














